/* example "simple_client.c" from jackaudio.github.com */

/** @file simple_client.c
 *
 * @brief This simple client demonstrates the most basic features of JACK
 * as they would be used by many applications.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <jack/jack.h>

#define OUTFILENAME "rawOut.csv"
#define OUTFILEPATH "/home/k-jax/Desktop/"

typedef enum bool
{
	TRUE = 1,
	FALSE = 0
} BOOL;

typedef struct asyncObj
{
	BOOL lockFlag;
	BOOL buffFull;
	uint32_t missCnt;
	uint32_t nSamples;
	uint32_t bufSize;
	jack_default_audio_sample_t * buff;
} AsyncObj;

jack_port_t *input_port;
jack_port_t *output_port;
jack_client_t *client;
AsyncObj * ObjPtr = NULL;

//This is our "workhorse", it's called from jack periodically for each block of data. Optimize for time...

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
 * the user ( e.g. using Ctrl-C on a unix-ish operating system)
 */
int process(jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *in, *out;

    in = jack_port_get_buffer(input_port, nframes);
    out = jack_port_get_buffer(output_port, nframes);
    memcpy(out, in, sizeof(jack_default_audio_sample_t) * nframes);
	if(!ObjPtr)
	{
		return 0;//1; // error! no async obj!
	}
	if(ObjPtr->lockFlag)
	{
    	ObjPtr->missCnt += 1; // we just dropped a frame!
	}
	else
	{
		ObjPtr->lockFlag = 1; // lock out access to our shared object
		memcpy(ObjPtr->buff,in,sizeof(jack_default_audio_sample_t)*nframes);
		ObjPtr->nSamples = nframes;
		ObjPtr->buffFull = TRUE;
		ObjPtr->lockFlag = 0; // clear lock out flag, our work here is finished!
	}

    return 0;
}

//jack communicates through callbacks - this one will kill us :(

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown(void *arg)
{
    exit(1);
}


//// kj wrote this function
//uint32_t sumJackArray(jack_default_audio_sample_t * ar, uint32_t size_ar)
 //{
//	uint32_t i;
//	uint32_t sum;
//
//	if(!ar)
//	{
//		fprintf(stdout,"error computing sum! array was NULL pointer!\n");
//		return 0; // get out of here before we hurt ourselves!
//	}
//	sum = 0;
//	for(i = 0; i < size_ar; i++)
//	{
//		sum += ar[i];
//	}
//	return sum;
//}
//// kj wrote this other function
//uint32_t EstimateInstEnergy(jack_default_audio_sample_t * ain, uint32_t size_ain

// kj worte this function too
int8_t close_up_shop(jack_client_t * clientPtr, AsyncObj * AObj, FILE * filePointer, int8_t exitCode)
{
	if(clientPtr)
	{
		jack_client_close(clientPtr);
	}
	if(AObj)
	{
		if(AObj->buff)
		{
			free(AObj->buff);
		}
	}
	if(filePointer)
	{
		fclose(filePointer);
	}
	exit(exitCode);
}

//Here we go - our main program...

int main(int argc, char *argv[])
{
    const char **ports;
    const char *client_name = "simple";
    const char *server_name = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;
	AsyncObj AsyncBufObj;
	uint32_t nSamples;
	uint32_t prevMissCnt;
	char fileName[50]; // ensure fileName + filePath is no more than 50 characters!
	FILE * fp;
	uint16_t i;

    /* open a client connection to the JACK server */

    client = jack_client_open(client_name, options, &status, server_name);
    if(client == NULL)
	{
        fprintf(stderr, "jack_client_open() failed, "
             "status = 0x%2.0x\n", status);
        if(status & JackServerFailed)
		{
            fprintf(stderr, "Unable to connect to JACK server\n");
        }
        exit(1);
    }
    if(status & JackServerStarted)
	{
        fprintf(stderr, "JACK server started\n");
    }
    if(status & JackNameNotUnique)
	{
        client_name = jack_get_client_name(client);
        fprintf(stderr, "unique name `%s' assigned\n", client_name);
    }

    /* tell the JACK server to call `process()' whenever
       there is work to be done.
    */

    jack_set_process_callback(client, process, 0);

    /* tell the JACK server to call `jack_shutdown()' if
       it ever shuts down, either entirely, or if it
       just decides to stop calling us.
    */

    jack_on_shutdown(client, jack_shutdown, 0);

    /* display the current sample rate. 
     */

    printf("engine sample rate: %" PRIu32 "\n", jack_get_sample_rate(client));

    /* create two ports */

    input_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    output_port = jack_port_register(client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if((input_port == NULL) ||(output_port == NULL))
	{
        fprintf(stderr, "no more JACK ports available\n");
        exit(1);
    }

    /* Tell the JACK server that we are ready to roll.  Our
     * process() callback will start running now. */

    if(jack_activate(client))
	{
        fprintf(stderr, "cannot activate client");
        exit(1);
    }
//The next part should be optional - it's alway a bad thing to connect ports automatically.

    /* Connect the ports.  You can't do this before the client is
     * activated, because we can't make connections to clients
     * that aren't running.  Note the confusing(but necessary)
     * orientation of the driver backend ports: playback ports are
     * "input" to the backend, and capture ports are "output" from
     * it.
     */

    ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput);
    if(ports == NULL)
	{
        fprintf(stderr, "no physical capture ports\n");
        exit(1);
    }

    if(jack_connect(client, ports[0], jack_port_name(input_port)))
	{
        fprintf(stderr, "cannot connect input ports\n");
    }

    free(ports);

    ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if(ports == NULL)
	{
        fprintf(stderr, "no physical playback ports\n");
        exit(1);
    }

    if(jack_connect(client, jack_port_name(output_port), ports[0]))
	{
        fprintf(stderr, "cannot connect output ports\n");
    }

    free(ports);

//If you kill this program you'll see xruns for sure. Better call jack_client_close( client ) via atexit(..).
    
    /* initialize asynchronous object */
    AsyncBufObj.lockFlag = 0;
	AsyncBufObj.buffFull = FALSE;
    AsyncBufObj.missCnt = 0;
	AsyncBufObj.bufSize = (uint32_t) jack_port_type_get_buffer_size(client,JACK_DEFAULT_AUDIO_TYPE);
	fprintf(stdout,"creating %d length array of %d byte sized elements for async buff\n",AsyncBufObj.bufSize,sizeof(jack_default_audio_sample_t));
	AsyncBufObj.buff = (jack_default_audio_sample_t*) malloc(AsyncBufObj.bufSize*sizeof(jack_default_audio_sample_t)); // allocate memory for our async buffer
	if(!AsyncBufObj.buff) // memory was not given to us!
	{
		fprintf(stderr,"malloc: memory for async buff not allocated!\n");
		close_up_shop(client,0,0,-1); // exit with error
		return; // shouldn't ever get here, but just in case
	}
	ObjPtr = &AsyncBufObj; // after initialization, assign pointer to our object

	/* open output file to dump raw samples */
	if(sprintf(fileName,"%s%s",OUTFILEPATH,OUTFILENAME));
	{
		fp = fopen(fileName,"w");
	}
	
	/* check if file opened properly */
	if(!fp)
	{
		fprintf(stderr,"file could not be opened!\nfilename: %s\n",fileName);
		close_up_shop(client,&AsyncBufObj,0,-1); // exit with error
		return; // shouldn't ever get here, but just in case
	}
	
	/* keep running until stopped by the user */
	prevMissCnt = 0; // this is temporary and used to monotor if frames are getting dropped!
	while(1) // THIS IS THE MAIN LOOP
	{
		if(AsyncBufObj.buffFull)
		{
			AsyncBufObj.lockFlag = 1; // set lock so callback doesn't change data on us while processing
			for(i = 0; i < AsyncBufObj.nSamples-1; i++)
			{
    			fprintf(fp,"%f,",(float)AsyncBufObj.buff[i]); // dump samples to file
			}
			fprintf(fp,"%f\n",(float)AsyncBufObj.buff[i]); // dump last sample to file w/ out comma for proper csv formatting
			if(AsyncBufObj.missCnt != prevMissCnt) // check if a frame was dropped
			{
				fprintf(stdout,"missCnt = %d\n",AsyncBufObj.missCnt); // notify user we dropped a frame!
				prevMissCnt = AsyncBufObj.missCnt;
			}
			AsyncBufObj.buffFull = FALSE; // clear buffFull flag
			AsyncBufObj.lockFlag = 0; // clear lock flag, we got the stuff!
			//fprintf(stdout,"%d\n",nSamples); // display number of samples placed in our async buff
		}
	}

    /* this is never reached but if the program
       had some other way to exit besides being killed,
       they would be important to call.
    */

	close_up_shop(client,&AsyncBufObj,fp,0); // exit with no errors!
	return; // shouldn't ever get here, but just in case
}
