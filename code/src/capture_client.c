<!doctype html public "-//W3C//DTD HTML 3.2//EN">
<html>
<head>
<title>JACK/example-clients/capture_client.c</title>
<base href="http://jackit.sourceforge.net/cgi-bin/lxr/http/">
</head>

<body bgcolor=white>
<div align=center>
   ~ <b>[</b>&nbsp;<b><i>source navigation</i></b>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="diff/example-clients/capture_client.c">diff markup</a>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="ident">identifier search</a>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="search">freetext search</a>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="find">file search</a>&nbsp;<b>]</b> ~
</div>

<h1 align=center>
  <a href="http://www.linux.org/"> 
    Linux</a>
  <a href="http:blurb.html">
    Cross Reference</a><br>
  <a href="source/">JACK</a>/<a href="source/example-clients/">example-clients</a>/<a href="source/example-clients/capture_client.c">capture_client.c</a>
</h1>

<div align=center>
  
</div>
<hr>
<pre><h4 align="center"><i>** Warning: Cannot open xref database.</i></h4>
  <a name=L1 href="source/example-clients/capture_client.c#L1">1</a> <b><i>/*</i></b>
  <a name=L2 href="source/example-clients/capture_client.c#L2">2</a> <b><i>    Copyright (C) 2001 Paul Davis</i></b>
  <a name=L3 href="source/example-clients/capture_client.c#L3">3</a> <b><i>    Copyright (C) 2003 Jack O'Quin</i></b>
  <a name=L4 href="source/example-clients/capture_client.c#L4">4</a> <b><i>    </i></b>
  <a name=L5 href="source/example-clients/capture_client.c#L5">5</a> <b><i>    This program is free software; you can redistribute it and/or modify</i></b>
  <a name=L6 href="source/example-clients/capture_client.c#L6">6</a> <b><i>    it under the terms of the GNU General Public License as published by</i></b>
  <a name=L7 href="source/example-clients/capture_client.c#L7">7</a> <b><i>    the Free Software Foundation; either version 2 of the License, or</i></b>
  <a name=L8 href="source/example-clients/capture_client.c#L8">8</a> <b><i>    (at your option) any later version.</i></b>
  <a name=L9 href="source/example-clients/capture_client.c#L9">9</a> <b><i></i></b>
 <a name=L10 href="source/example-clients/capture_client.c#L10">10</a> <b><i>    This program is distributed in the hope that it will be useful,</i></b>
 <a name=L11 href="source/example-clients/capture_client.c#L11">11</a> <b><i>    but WITHOUT ANY WARRANTY; without even the implied warranty of</i></b>
 <a name=L12 href="source/example-clients/capture_client.c#L12">12</a> <b><i>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the</i></b>
 <a name=L13 href="source/example-clients/capture_client.c#L13">13</a> <b><i>    GNU General Public License for more details.</i></b>
 <a name=L14 href="source/example-clients/capture_client.c#L14">14</a> <b><i></i></b>
 <a name=L15 href="source/example-clients/capture_client.c#L15">15</a> <b><i>    You should have received a copy of the GNU General Public License</i></b>
 <a name=L16 href="source/example-clients/capture_client.c#L16">16</a> <b><i>    along with this program; if not, write to the Free Software</i></b>
 <a name=L17 href="source/example-clients/capture_client.c#L17">17</a> <b><i>    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.</i></b>
 <a name=L18 href="source/example-clients/capture_client.c#L18">18</a> <b><i></i></b>
 <a name=L19 href="source/example-clients/capture_client.c#L19">19</a> <b><i>    * 2002/08/23 - modify for libsndfile 1.0.0 &lt;andy@alsaplayer.org&gt;</i></b>
 <a name=L20 href="source/example-clients/capture_client.c#L20">20</a> <b><i>    * 2003/05/26 - use ringbuffers - joq</i></b>
 <a name=L21 href="source/example-clients/capture_client.c#L21">21</a> <b><i>    </i></b>
 <a name=L22 href="source/example-clients/capture_client.c#L22">22</a> <b><i>    $Id: capture_client.c,v 1.9 2003/10/31 20:52:23 joq Exp $</i></b>
 <a name=L23 href="source/example-clients/capture_client.c#L23">23</a> <b><i>*/</i></b>
 <a name=L24 href="source/example-clients/capture_client.c#L24">24</a> 
 <a name=L25 href="source/example-clients/capture_client.c#L25">25</a> #include &lt;stdio.h&gt;
 <a name=L26 href="source/example-clients/capture_client.c#L26">26</a> #include &lt;stdlib.h&gt;
 <a name=L27 href="source/example-clients/capture_client.c#L27">27</a> #include &lt;string.h&gt;
 <a name=L28 href="source/example-clients/capture_client.c#L28">28</a> #include &lt;errno.h&gt;
 <a name=L29 href="source/example-clients/capture_client.c#L29">29</a> #include &lt;unistd.h&gt;
 <a name=L30 href="source/example-clients/capture_client.c#L30">30</a> #include &lt;sndfile.h&gt;
 <a name=L31 href="source/example-clients/capture_client.c#L31">31</a> #include &lt;pthread.h&gt;
 <a name=L32 href="source/example-clients/capture_client.c#L32">32</a> #include &lt;getopt.h&gt;
 <a name=L33 href="source/example-clients/capture_client.c#L33">33</a> #include &lt;jack/jack.h&gt;
 <a name=L34 href="source/example-clients/capture_client.c#L34">34</a> #include &lt;jack/ringbuffer.h&gt;
 <a name=L35 href="source/example-clients/capture_client.c#L35">35</a> 
 <a name=L36 href="source/example-clients/capture_client.c#L36">36</a> typedef struct _thread_info {
 <a name=L37 href="source/example-clients/capture_client.c#L37">37</a>     pthread_t thread_id;
 <a name=L38 href="source/example-clients/capture_client.c#L38">38</a>     SNDFILE *sf;
 <a name=L39 href="source/example-clients/capture_client.c#L39">39</a>     jack_nframes_t duration;
 <a name=L40 href="source/example-clients/capture_client.c#L40">40</a>     jack_nframes_t rb_size;
 <a name=L41 href="source/example-clients/capture_client.c#L41">41</a>     jack_client_t *client;
 <a name=L42 href="source/example-clients/capture_client.c#L42">42</a>     unsigned int channels;
 <a name=L43 href="source/example-clients/capture_client.c#L43">43</a>     int bitdepth;
 <a name=L44 href="source/example-clients/capture_client.c#L44">44</a>     char *path;
 <a name=L45 href="source/example-clients/capture_client.c#L45">45</a>     volatile int can_capture;
 <a name=L46 href="source/example-clients/capture_client.c#L46">46</a>     volatile int can_process;
 <a name=L47 href="source/example-clients/capture_client.c#L47">47</a>     volatile int status;
 <a name=L48 href="source/example-clients/capture_client.c#L48">48</a> } thread_info_t;
 <a name=L49 href="source/example-clients/capture_client.c#L49">49</a> 
 <a name=L50 href="source/example-clients/capture_client.c#L50">50</a> <b><i>/* JACK data */</i></b>
 <a name=L51 href="source/example-clients/capture_client.c#L51">51</a> unsigned int nports;
 <a name=L52 href="source/example-clients/capture_client.c#L52">52</a> jack_port_t **ports;
 <a name=L53 href="source/example-clients/capture_client.c#L53">53</a> jack_default_audio_sample_t **in;
 <a name=L54 href="source/example-clients/capture_client.c#L54">54</a> jack_nframes_t nframes;
 <a name=L55 href="source/example-clients/capture_client.c#L55">55</a> const size_t sample_size = sizeof(jack_default_audio_sample_t);
 <a name=L56 href="source/example-clients/capture_client.c#L56">56</a> 
 <a name=L57 href="source/example-clients/capture_client.c#L57">57</a> <b><i>/* Synchronization between process thread and disk thread. */</i></b>
 <a name=L58 href="source/example-clients/capture_client.c#L58">58</a> #define DEFAULT_RB_SIZE 16384           <b><i>/* ringbuffer size in frames */</i></b>
 <a name=L59 href="source/example-clients/capture_client.c#L59">59</a> jack_ringbuffer_t *rb;
 <a name=L60 href="source/example-clients/capture_client.c#L60">60</a> pthread_mutex_t disk_thread_lock = PTHREAD_MUTEX_INITIALIZER;
 <a name=L61 href="source/example-clients/capture_client.c#L61">61</a> pthread_cond_t  data_ready = PTHREAD_COND_INITIALIZER;
 <a name=L62 href="source/example-clients/capture_client.c#L62">62</a> long overruns = 0;
 <a name=L63 href="source/example-clients/capture_client.c#L63">63</a> 
 <a name=L64 href="source/example-clients/capture_client.c#L64">64</a> 
 <a name=L65 href="source/example-clients/capture_client.c#L65">65</a> void *
 <a name=L66 href="source/example-clients/capture_client.c#L66">66</a> disk_thread (void *arg)
 <a name=L67 href="source/example-clients/capture_client.c#L67">67</a> {
 <a name=L68 href="source/example-clients/capture_client.c#L68">68</a>         thread_info_t *info = (thread_info_t *) arg;
 <a name=L69 href="source/example-clients/capture_client.c#L69">69</a>         static jack_nframes_t total_captured = 0;
 <a name=L70 href="source/example-clients/capture_client.c#L70">70</a>         jack_nframes_t samples_per_frame = info-&gt;channels;
 <a name=L71 href="source/example-clients/capture_client.c#L71">71</a>         size_t bytes_per_frame = samples_per_frame * sample_size;
 <a name=L72 href="source/example-clients/capture_client.c#L72">72</a>         void *framebuf = malloc (bytes_per_frame);
 <a name=L73 href="source/example-clients/capture_client.c#L73">73</a> 
 <a name=L74 href="source/example-clients/capture_client.c#L74">74</a>         pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 <a name=L75 href="source/example-clients/capture_client.c#L75">75</a>         pthread_mutex_lock (&amp;disk_thread_lock);
 <a name=L76 href="source/example-clients/capture_client.c#L76">76</a> 
 <a name=L77 href="source/example-clients/capture_client.c#L77">77</a>         info-&gt;status = 0;
 <a name=L78 href="source/example-clients/capture_client.c#L78">78</a> 
 <a name=L79 href="source/example-clients/capture_client.c#L79">79</a>         while (1) {
 <a name=L80 href="source/example-clients/capture_client.c#L80">80</a> 
 <a name=L81 href="source/example-clients/capture_client.c#L81">81</a>                 <b><i>/* Write the data one frame at a time.  This is</i></b>
 <a name=L82 href="source/example-clients/capture_client.c#L82">82</a> <b><i>                 * inefficient, but makes things simpler. */</i></b>
 <a name=L83 href="source/example-clients/capture_client.c#L83">83</a>                 while (info-&gt;can_capture &amp;&amp;
 <a name=L84 href="source/example-clients/capture_client.c#L84">84</a>                        (jack_ringbuffer_read_space (rb) &gt;= bytes_per_frame)) {
 <a name=L85 href="source/example-clients/capture_client.c#L85">85</a> 
 <a name=L86 href="source/example-clients/capture_client.c#L86">86</a>                         jack_ringbuffer_read (rb, framebuf, bytes_per_frame);
 <a name=L87 href="source/example-clients/capture_client.c#L87">87</a> 
 <a name=L88 href="source/example-clients/capture_client.c#L88">88</a>                         if (sf_writef_float (info-&gt;sf, framebuf, 1) != 1) {
 <a name=L89 href="source/example-clients/capture_client.c#L89">89</a>                                 char errstr[256];
 <a name=L90 href="source/example-clients/capture_client.c#L90">90</a>                                 sf_error_str (0, errstr, sizeof (errstr) - 1);
 <a name=L91 href="source/example-clients/capture_client.c#L91">91</a>                                 fprintf (stderr,
 <a name=L92 href="source/example-clients/capture_client.c#L92">92</a>                                          <i>"cannot write sndfile (%s)\n"</i>,
 <a name=L93 href="source/example-clients/capture_client.c#L93">93</a>                                          errstr);
 <a name=L94 href="source/example-clients/capture_client.c#L94">94</a>                                 info-&gt;status = EIO; <b><i>/* write failed */</i></b>
 <a name=L95 href="source/example-clients/capture_client.c#L95">95</a>                                 goto done;
 <a name=L96 href="source/example-clients/capture_client.c#L96">96</a>                         }
 <a name=L97 href="source/example-clients/capture_client.c#L97">97</a>                                 
 <a name=L98 href="source/example-clients/capture_client.c#L98">98</a>                         if (++total_captured &gt;= info-&gt;duration) {
 <a name=L99 href="source/example-clients/capture_client.c#L99">99</a>                                 printf (<i>"disk thread finished\n"</i>);
<a name=L100 href="source/example-clients/capture_client.c#L100">100</a>                                 goto done;
<a name=L101 href="source/example-clients/capture_client.c#L101">101</a>                         }
<a name=L102 href="source/example-clients/capture_client.c#L102">102</a>                 }
<a name=L103 href="source/example-clients/capture_client.c#L103">103</a> 
<a name=L104 href="source/example-clients/capture_client.c#L104">104</a>                 <b><i>/* wait until process() signals more data */</i></b>
<a name=L105 href="source/example-clients/capture_client.c#L105">105</a>                 pthread_cond_wait (&amp;data_ready, &amp;disk_thread_lock);
<a name=L106 href="source/example-clients/capture_client.c#L106">106</a>         }
<a name=L107 href="source/example-clients/capture_client.c#L107">107</a> 
<a name=L108 href="source/example-clients/capture_client.c#L108">108</a>  done:
<a name=L109 href="source/example-clients/capture_client.c#L109">109</a>         pthread_mutex_unlock (&amp;disk_thread_lock);
<a name=L110 href="source/example-clients/capture_client.c#L110">110</a>         free (framebuf);
<a name=L111 href="source/example-clients/capture_client.c#L111">111</a>         return 0;
<a name=L112 href="source/example-clients/capture_client.c#L112">112</a> }
<a name=L113 href="source/example-clients/capture_client.c#L113">113</a>         
<a name=L114 href="source/example-clients/capture_client.c#L114">114</a> int
<a name=L115 href="source/example-clients/capture_client.c#L115">115</a> process (jack_nframes_t nframes, void *arg)
<a name=L116 href="source/example-clients/capture_client.c#L116">116</a> {
<a name=L117 href="source/example-clients/capture_client.c#L117">117</a>         int chn;
<a name=L118 href="source/example-clients/capture_client.c#L118">118</a>         size_t i;
<a name=L119 href="source/example-clients/capture_client.c#L119">119</a>         thread_info_t *info = (thread_info_t *) arg;
<a name=L120 href="source/example-clients/capture_client.c#L120">120</a> 
<a name=L121 href="source/example-clients/capture_client.c#L121">121</a>         <b><i>/* Do nothing until we're ready to begin. */</i></b>
<a name=L122 href="source/example-clients/capture_client.c#L122">122</a>         if ((!info-&gt;can_process) || (!info-&gt;can_capture))
<a name=L123 href="source/example-clients/capture_client.c#L123">123</a>                 return 0;
<a name=L124 href="source/example-clients/capture_client.c#L124">124</a> 
<a name=L125 href="source/example-clients/capture_client.c#L125">125</a>         for (chn = 0; chn &lt; nports; chn++)
<a name=L126 href="source/example-clients/capture_client.c#L126">126</a>                 in[chn] = jack_port_get_buffer (ports[chn], nframes);
<a name=L127 href="source/example-clients/capture_client.c#L127">127</a> 
<a name=L128 href="source/example-clients/capture_client.c#L128">128</a>         <b><i>/* Sndfile requires interleaved data.  It is simpler here to</i></b>
<a name=L129 href="source/example-clients/capture_client.c#L129">129</a> <b><i>         * just queue interleaved samples to a single ringbuffer. */</i></b>
<a name=L130 href="source/example-clients/capture_client.c#L130">130</a>         for (i = 0; i &lt; nframes; i++) {
<a name=L131 href="source/example-clients/capture_client.c#L131">131</a>                 for (chn = 0; chn &lt; nports; chn++) {
<a name=L132 href="source/example-clients/capture_client.c#L132">132</a>                         if (jack_ringbuffer_write (rb, (void *) (in[chn]+i),
<a name=L133 href="source/example-clients/capture_client.c#L133">133</a>                                               sample_size)
<a name=L134 href="source/example-clients/capture_client.c#L134">134</a>                             &lt; sample_size)
<a name=L135 href="source/example-clients/capture_client.c#L135">135</a>                                 overruns++;
<a name=L136 href="source/example-clients/capture_client.c#L136">136</a>                 }
<a name=L137 href="source/example-clients/capture_client.c#L137">137</a>         }
<a name=L138 href="source/example-clients/capture_client.c#L138">138</a> 
<a name=L139 href="source/example-clients/capture_client.c#L139">139</a>         <b><i>/* Tell the disk thread there is work to do.  If it is already</i></b>
<a name=L140 href="source/example-clients/capture_client.c#L140">140</a> <b><i>         * running, the lock will not be available.  We can't wait</i></b>
<a name=L141 href="source/example-clients/capture_client.c#L141">141</a> <b><i>         * here in the process() thread, but we don't need to signal</i></b>
<a name=L142 href="source/example-clients/capture_client.c#L142">142</a> <b><i>         * in that case, because the disk thread will read all the</i></b>
<a name=L143 href="source/example-clients/capture_client.c#L143">143</a> <b><i>         * data queued before waiting again. */</i></b>
<a name=L144 href="source/example-clients/capture_client.c#L144">144</a>         if (pthread_mutex_trylock (&amp;disk_thread_lock) == 0) {
<a name=L145 href="source/example-clients/capture_client.c#L145">145</a>             pthread_cond_signal (&amp;data_ready);
<a name=L146 href="source/example-clients/capture_client.c#L146">146</a>             pthread_mutex_unlock (&amp;disk_thread_lock);
<a name=L147 href="source/example-clients/capture_client.c#L147">147</a>         }
<a name=L148 href="source/example-clients/capture_client.c#L148">148</a> 
<a name=L149 href="source/example-clients/capture_client.c#L149">149</a>         return 0;
<a name=L150 href="source/example-clients/capture_client.c#L150">150</a> }
<a name=L151 href="source/example-clients/capture_client.c#L151">151</a> 
<a name=L152 href="source/example-clients/capture_client.c#L152">152</a> void
<a name=L153 href="source/example-clients/capture_client.c#L153">153</a> jack_shutdown (void *arg)
<a name=L154 href="source/example-clients/capture_client.c#L154">154</a> {
<a name=L155 href="source/example-clients/capture_client.c#L155">155</a>         fprintf (stderr, <i>"JACK shutdown\n"</i>);
<a name=L156 href="source/example-clients/capture_client.c#L156">156</a>         <b><i>// exit (0);</i></b>
<a name=L157 href="source/example-clients/capture_client.c#L157">157</a> <b><i></i></b>        abort();
<a name=L158 href="source/example-clients/capture_client.c#L158">158</a> }
<a name=L159 href="source/example-clients/capture_client.c#L159">159</a> 
<a name=L160 href="source/example-clients/capture_client.c#L160">160</a> void
<a name=L161 href="source/example-clients/capture_client.c#L161">161</a> setup_disk_thread (thread_info_t *info)
<a name=L162 href="source/example-clients/capture_client.c#L162">162</a> {
<a name=L163 href="source/example-clients/capture_client.c#L163">163</a>         SF_INFO sf_info;
<a name=L164 href="source/example-clients/capture_client.c#L164">164</a>         int short_mask;
<a name=L165 href="source/example-clients/capture_client.c#L165">165</a>         
<a name=L166 href="source/example-clients/capture_client.c#L166">166</a>         sf_info.samplerate = jack_get_sample_rate (info-&gt;client);
<a name=L167 href="source/example-clients/capture_client.c#L167">167</a>         sf_info.channels = info-&gt;channels;
<a name=L168 href="source/example-clients/capture_client.c#L168">168</a>         
<a name=L169 href="source/example-clients/capture_client.c#L169">169</a>         switch (info-&gt;bitdepth) {
<a name=L170 href="source/example-clients/capture_client.c#L170">170</a>                 case 8: short_mask = SF_FORMAT_PCM_U8;
<a name=L171 href="source/example-clients/capture_client.c#L171">171</a>                         break;
<a name=L172 href="source/example-clients/capture_client.c#L172">172</a>                 case 16: short_mask = SF_FORMAT_PCM_16;
<a name=L173 href="source/example-clients/capture_client.c#L173">173</a>                          break;
<a name=L174 href="source/example-clients/capture_client.c#L174">174</a>                 case 24: short_mask = SF_FORMAT_PCM_24;
<a name=L175 href="source/example-clients/capture_client.c#L175">175</a>                          break;
<a name=L176 href="source/example-clients/capture_client.c#L176">176</a>                 case 32: short_mask = SF_FORMAT_PCM_32;
<a name=L177 href="source/example-clients/capture_client.c#L177">177</a>                          break;
<a name=L178 href="source/example-clients/capture_client.c#L178">178</a>                 default: short_mask = SF_FORMAT_PCM_16;
<a name=L179 href="source/example-clients/capture_client.c#L179">179</a>                          break;
<a name=L180 href="source/example-clients/capture_client.c#L180">180</a>         }                
<a name=L181 href="source/example-clients/capture_client.c#L181">181</a>         sf_info.format = SF_FORMAT_WAV|short_mask;
<a name=L182 href="source/example-clients/capture_client.c#L182">182</a> 
<a name=L183 href="source/example-clients/capture_client.c#L183">183</a>         if ((info-&gt;sf = sf_open (info-&gt;path, SFM_WRITE, &amp;sf_info)) == NULL) {
<a name=L184 href="source/example-clients/capture_client.c#L184">184</a>                 char errstr[256];
<a name=L185 href="source/example-clients/capture_client.c#L185">185</a>                 sf_error_str (0, errstr, sizeof (errstr) - 1);
<a name=L186 href="source/example-clients/capture_client.c#L186">186</a>                 fprintf (stderr, <i>"cannot open sndfile \"%s\" for output (%s)\n"</i>, info-&gt;path, errstr);
<a name=L187 href="source/example-clients/capture_client.c#L187">187</a>                 jack_client_close (info-&gt;client);
<a name=L188 href="source/example-clients/capture_client.c#L188">188</a>                 exit (1);
<a name=L189 href="source/example-clients/capture_client.c#L189">189</a>         }
<a name=L190 href="source/example-clients/capture_client.c#L190">190</a> 
<a name=L191 href="source/example-clients/capture_client.c#L191">191</a>         info-&gt;duration *= sf_info.samplerate;
<a name=L192 href="source/example-clients/capture_client.c#L192">192</a>         info-&gt;can_capture = 0;
<a name=L193 href="source/example-clients/capture_client.c#L193">193</a> 
<a name=L194 href="source/example-clients/capture_client.c#L194">194</a>         pthread_create (&amp;info-&gt;thread_id, NULL, disk_thread, info);
<a name=L195 href="source/example-clients/capture_client.c#L195">195</a> }
<a name=L196 href="source/example-clients/capture_client.c#L196">196</a> 
<a name=L197 href="source/example-clients/capture_client.c#L197">197</a> void
<a name=L198 href="source/example-clients/capture_client.c#L198">198</a> run_disk_thread (thread_info_t *info)
<a name=L199 href="source/example-clients/capture_client.c#L199">199</a> {
<a name=L200 href="source/example-clients/capture_client.c#L200">200</a>         info-&gt;can_capture = 1;
<a name=L201 href="source/example-clients/capture_client.c#L201">201</a>         pthread_join (info-&gt;thread_id, NULL);
<a name=L202 href="source/example-clients/capture_client.c#L202">202</a>         sf_close (info-&gt;sf);
<a name=L203 href="source/example-clients/capture_client.c#L203">203</a>         if (overruns &gt; 0) {
<a name=L204 href="source/example-clients/capture_client.c#L204">204</a>                 fprintf (stderr,
<a name=L205 href="source/example-clients/capture_client.c#L205">205</a>                          <i>"jackrec failed with %ld overruns.\n"</i>, overruns);
<a name=L206 href="source/example-clients/capture_client.c#L206">206</a>                 fprintf (stderr, <i>" try a bigger buffer than -B %"</i>
<a name=L207 href="source/example-clients/capture_client.c#L207">207</a>                          PRIu32 <i>".\n"</i>, info-&gt;rb_size);
<a name=L208 href="source/example-clients/capture_client.c#L208">208</a>                 info-&gt;status = EPIPE;
<a name=L209 href="source/example-clients/capture_client.c#L209">209</a>         }
<a name=L210 href="source/example-clients/capture_client.c#L210">210</a>         if (info-&gt;status) {
<a name=L211 href="source/example-clients/capture_client.c#L211">211</a>                 unlink (info-&gt;path);
<a name=L212 href="source/example-clients/capture_client.c#L212">212</a>         }
<a name=L213 href="source/example-clients/capture_client.c#L213">213</a> }
<a name=L214 href="source/example-clients/capture_client.c#L214">214</a> 
<a name=L215 href="source/example-clients/capture_client.c#L215">215</a> void
<a name=L216 href="source/example-clients/capture_client.c#L216">216</a> setup_ports (int sources, char *source_names[], thread_info_t *info)
<a name=L217 href="source/example-clients/capture_client.c#L217">217</a> {
<a name=L218 href="source/example-clients/capture_client.c#L218">218</a>         unsigned int i;
<a name=L219 href="source/example-clients/capture_client.c#L219">219</a>         size_t in_size;
<a name=L220 href="source/example-clients/capture_client.c#L220">220</a> 
<a name=L221 href="source/example-clients/capture_client.c#L221">221</a>         <b><i>/* Allocate data structures that depend on the number of ports. */</i></b>
<a name=L222 href="source/example-clients/capture_client.c#L222">222</a>         nports = sources;
<a name=L223 href="source/example-clients/capture_client.c#L223">223</a>         ports = (jack_port_t **) malloc (sizeof (jack_port_t *) * nports);
<a name=L224 href="source/example-clients/capture_client.c#L224">224</a>         in_size =  nports * sizeof (jack_default_audio_sample_t *);
<a name=L225 href="source/example-clients/capture_client.c#L225">225</a>         in = (jack_default_audio_sample_t **) malloc (in_size);
<a name=L226 href="source/example-clients/capture_client.c#L226">226</a>         rb = jack_ringbuffer_create (nports * sample_size * info-&gt;rb_size);
<a name=L227 href="source/example-clients/capture_client.c#L227">227</a> 
<a name=L228 href="source/example-clients/capture_client.c#L228">228</a>         <b><i>/* When JACK is running realtime, jack_activate() will have</i></b>
<a name=L229 href="source/example-clients/capture_client.c#L229">229</a> <b><i>         * called mlockall() to lock our pages into memory.  But, we</i></b>
<a name=L230 href="source/example-clients/capture_client.c#L230">230</a> <b><i>         * still need to touch any newly allocated pages before</i></b>
<a name=L231 href="source/example-clients/capture_client.c#L231">231</a> <b><i>         * process() starts using them.  Otherwise, a page fault could</i></b>
<a name=L232 href="source/example-clients/capture_client.c#L232">232</a> <b><i>         * create a delay that would force JACK to shut us down. */</i></b>
<a name=L233 href="source/example-clients/capture_client.c#L233">233</a>         memset(in, 0, in_size);
<a name=L234 href="source/example-clients/capture_client.c#L234">234</a>         memset(rb-&gt;buf, 0, rb-&gt;size);
<a name=L235 href="source/example-clients/capture_client.c#L235">235</a> 
<a name=L236 href="source/example-clients/capture_client.c#L236">236</a>         for (i = 0; i &lt; nports; i++) {
<a name=L237 href="source/example-clients/capture_client.c#L237">237</a>                 char name[64];
<a name=L238 href="source/example-clients/capture_client.c#L238">238</a> 
<a name=L239 href="source/example-clients/capture_client.c#L239">239</a>                 sprintf (name, <i>"input%d"</i>, i+1);
<a name=L240 href="source/example-clients/capture_client.c#L240">240</a> 
<a name=L241 href="source/example-clients/capture_client.c#L241">241</a>                 if ((ports[i] = jack_port_register (info-&gt;client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0)) == 0) {
<a name=L242 href="source/example-clients/capture_client.c#L242">242</a>                         fprintf (stderr, <i>"cannot register input port \"%s\"!\n"</i>, name);
<a name=L243 href="source/example-clients/capture_client.c#L243">243</a>                         jack_client_close (info-&gt;client);
<a name=L244 href="source/example-clients/capture_client.c#L244">244</a>                         exit (1);
<a name=L245 href="source/example-clients/capture_client.c#L245">245</a>                 }
<a name=L246 href="source/example-clients/capture_client.c#L246">246</a>         }
<a name=L247 href="source/example-clients/capture_client.c#L247">247</a> 
<a name=L248 href="source/example-clients/capture_client.c#L248">248</a>         for (i = 0; i &lt; nports; i++) {
<a name=L249 href="source/example-clients/capture_client.c#L249">249</a>                 if (jack_connect (info-&gt;client, source_names[i], jack_port_name (ports[i]))) {
<a name=L250 href="source/example-clients/capture_client.c#L250">250</a>                         fprintf (stderr, <i>"cannot connect input port %s to %s\n"</i>, jack_port_name (ports[i]), source_names[i]);
<a name=L251 href="source/example-clients/capture_client.c#L251">251</a>                         jack_client_close (info-&gt;client);
<a name=L252 href="source/example-clients/capture_client.c#L252">252</a>                         exit (1);
<a name=L253 href="source/example-clients/capture_client.c#L253">253</a>                 } 
<a name=L254 href="source/example-clients/capture_client.c#L254">254</a>         }
<a name=L255 href="source/example-clients/capture_client.c#L255">255</a> 
<a name=L256 href="source/example-clients/capture_client.c#L256">256</a>         info-&gt;can_process = 1;          <b><i>/* process() can start, now */</i></b>
<a name=L257 href="source/example-clients/capture_client.c#L257">257</a> }
<a name=L258 href="source/example-clients/capture_client.c#L258">258</a> 
<a name=L259 href="source/example-clients/capture_client.c#L259">259</a> int
<a name=L260 href="source/example-clients/capture_client.c#L260">260</a> main (int argc, char *argv[])
<a name=L261 href="source/example-clients/capture_client.c#L261">261</a> 
<a name=L262 href="source/example-clients/capture_client.c#L262">262</a> {
<a name=L263 href="source/example-clients/capture_client.c#L263">263</a>         jack_client_t *client;
<a name=L264 href="source/example-clients/capture_client.c#L264">264</a>         thread_info_t thread_info;
<a name=L265 href="source/example-clients/capture_client.c#L265">265</a>         int c;
<a name=L266 href="source/example-clients/capture_client.c#L266">266</a>         int longopt_index = 0;
<a name=L267 href="source/example-clients/capture_client.c#L267">267</a>         extern int optind, opterr;
<a name=L268 href="source/example-clients/capture_client.c#L268">268</a>         int show_usage = 0;
<a name=L269 href="source/example-clients/capture_client.c#L269">269</a>         char *optstring = <i>"d:f:b:B:h"</i>;
<a name=L270 href="source/example-clients/capture_client.c#L270">270</a>         struct option long_options[] = {
<a name=L271 href="source/example-clients/capture_client.c#L271">271</a>                 { <i>"help"</i>, 0, 0, <i>'h'</i> },
<a name=L272 href="source/example-clients/capture_client.c#L272">272</a>                 { <i>"duration"</i>, 1, 0, <i>'d'</i> },
<a name=L273 href="source/example-clients/capture_client.c#L273">273</a>                 { <i>"file"</i>, 1, 0, <i>'f'</i> },
<a name=L274 href="source/example-clients/capture_client.c#L274">274</a>                 { <i>"bitdepth"</i>, 1, 0, <i>'b'</i> },
<a name=L275 href="source/example-clients/capture_client.c#L275">275</a>                 { <i>"bufsize"</i>, 1, 0, <i>'B'</i> },
<a name=L276 href="source/example-clients/capture_client.c#L276">276</a>                 { 0, 0, 0, 0 }
<a name=L277 href="source/example-clients/capture_client.c#L277">277</a>         };
<a name=L278 href="source/example-clients/capture_client.c#L278">278</a> 
<a name=L279 href="source/example-clients/capture_client.c#L279">279</a>         memset (&amp;thread_info, 0, sizeof (thread_info));
<a name=L280 href="source/example-clients/capture_client.c#L280">280</a>         thread_info.rb_size = DEFAULT_RB_SIZE;
<a name=L281 href="source/example-clients/capture_client.c#L281">281</a>         opterr = 0;
<a name=L282 href="source/example-clients/capture_client.c#L282">282</a> 
<a name=L283 href="source/example-clients/capture_client.c#L283">283</a>         while ((c = getopt_long (argc, argv, optstring, long_options, &amp;longopt_index)) != -1) {
<a name=L284 href="source/example-clients/capture_client.c#L284">284</a>                 switch (c) {
<a name=L285 href="source/example-clients/capture_client.c#L285">285</a>                 case 1:
<a name=L286 href="source/example-clients/capture_client.c#L286">286</a>                         <b><i>/* getopt signals end of '-' options */</i></b>
<a name=L287 href="source/example-clients/capture_client.c#L287">287</a>                         break;
<a name=L288 href="source/example-clients/capture_client.c#L288">288</a> 
<a name=L289 href="source/example-clients/capture_client.c#L289">289</a>                 case <i>'h'</i>:
<a name=L290 href="source/example-clients/capture_client.c#L290">290</a>                         show_usage++;
<a name=L291 href="source/example-clients/capture_client.c#L291">291</a>                         break;
<a name=L292 href="source/example-clients/capture_client.c#L292">292</a>                 case <i>'d'</i>:
<a name=L293 href="source/example-clients/capture_client.c#L293">293</a>                         thread_info.duration = atoi (optarg);
<a name=L294 href="source/example-clients/capture_client.c#L294">294</a>                         break;
<a name=L295 href="source/example-clients/capture_client.c#L295">295</a>                 case <i>'f'</i>:
<a name=L296 href="source/example-clients/capture_client.c#L296">296</a>                         thread_info.path = optarg;
<a name=L297 href="source/example-clients/capture_client.c#L297">297</a>                         break;
<a name=L298 href="source/example-clients/capture_client.c#L298">298</a>                 case <i>'b'</i>:
<a name=L299 href="source/example-clients/capture_client.c#L299">299</a>                         thread_info.bitdepth = atoi (optarg);
<a name=L300 href="source/example-clients/capture_client.c#L300">300</a>                         break;
<a name=L301 href="source/example-clients/capture_client.c#L301">301</a>                 case <i>'B'</i>:
<a name=L302 href="source/example-clients/capture_client.c#L302">302</a>                         thread_info.rb_size = atoi (optarg);
<a name=L303 href="source/example-clients/capture_client.c#L303">303</a>                         break;
<a name=L304 href="source/example-clients/capture_client.c#L304">304</a>                 default:
<a name=L305 href="source/example-clients/capture_client.c#L305">305</a>                         fprintf (stderr, <i>"error\n"</i>);
<a name=L306 href="source/example-clients/capture_client.c#L306">306</a>                         show_usage++;
<a name=L307 href="source/example-clients/capture_client.c#L307">307</a>                         break;
<a name=L308 href="source/example-clients/capture_client.c#L308">308</a>                 }
<a name=L309 href="source/example-clients/capture_client.c#L309">309</a>         }
<a name=L310 href="source/example-clients/capture_client.c#L310">310</a> 
<a name=L311 href="source/example-clients/capture_client.c#L311">311</a>         if (show_usage || thread_info.path == NULL || optind == argc) {
<a name=L312 href="source/example-clients/capture_client.c#L312">312</a>                 fprintf (stderr, <i>"usage: jackrec -f filename [ -d second ] [ -b bitdepth ] [ -B bufsize ] port1 [ port2 ... ]\n"</i>);
<a name=L313 href="source/example-clients/capture_client.c#L313">313</a>                 exit (1);
<a name=L314 href="source/example-clients/capture_client.c#L314">314</a>         }
<a name=L315 href="source/example-clients/capture_client.c#L315">315</a> 
<a name=L316 href="source/example-clients/capture_client.c#L316">316</a>         if ((client = jack_client_new (<i>"jackrec"</i>)) == 0) {
<a name=L317 href="source/example-clients/capture_client.c#L317">317</a>                 fprintf (stderr, <i>"jack server not running?\n"</i>);
<a name=L318 href="source/example-clients/capture_client.c#L318">318</a>                 exit (1);
<a name=L319 href="source/example-clients/capture_client.c#L319">319</a>         }
<a name=L320 href="source/example-clients/capture_client.c#L320">320</a> 
<a name=L321 href="source/example-clients/capture_client.c#L321">321</a>         thread_info.client = client;
<a name=L322 href="source/example-clients/capture_client.c#L322">322</a>         thread_info.channels = argc - optind;
<a name=L323 href="source/example-clients/capture_client.c#L323">323</a>         thread_info.can_process = 0;
<a name=L324 href="source/example-clients/capture_client.c#L324">324</a> 
<a name=L325 href="source/example-clients/capture_client.c#L325">325</a>         setup_disk_thread (&amp;thread_info);
<a name=L326 href="source/example-clients/capture_client.c#L326">326</a> 
<a name=L327 href="source/example-clients/capture_client.c#L327">327</a>         jack_set_process_callback (client, process, &amp;thread_info);
<a name=L328 href="source/example-clients/capture_client.c#L328">328</a>         jack_on_shutdown (client, jack_shutdown, &amp;thread_info);
<a name=L329 href="source/example-clients/capture_client.c#L329">329</a> 
<a name=L330 href="source/example-clients/capture_client.c#L330">330</a>         if (jack_activate (client)) {
<a name=L331 href="source/example-clients/capture_client.c#L331">331</a>                 fprintf (stderr, <i>"cannot activate client"</i>);
<a name=L332 href="source/example-clients/capture_client.c#L332">332</a>         }
<a name=L333 href="source/example-clients/capture_client.c#L333">333</a> 
<a name=L334 href="source/example-clients/capture_client.c#L334">334</a>         setup_ports (argc - optind, &amp;argv[optind], &amp;thread_info);
<a name=L335 href="source/example-clients/capture_client.c#L335">335</a> 
<a name=L336 href="source/example-clients/capture_client.c#L336">336</a>         run_disk_thread (&amp;thread_info);
<a name=L337 href="source/example-clients/capture_client.c#L337">337</a> 
<a name=L338 href="source/example-clients/capture_client.c#L338">338</a>         jack_client_close (client);
<a name=L339 href="source/example-clients/capture_client.c#L339">339</a> 
<a name=L340 href="source/example-clients/capture_client.c#L340">340</a>         jack_ringbuffer_free (rb);
<a name=L341 href="source/example-clients/capture_client.c#L341">341</a> 
<a name=L342 href="source/example-clients/capture_client.c#L342">342</a>         exit (0);
<a name=L343 href="source/example-clients/capture_client.c#L343">343</a> }
<a name=L344 href="source/example-clients/capture_client.c#L344">344</a> </pre><hr>
<div align=center>
   ~ <b>[</b>&nbsp;<b><i>source navigation</i></b>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="diff/example-clients/capture_client.c">diff markup</a>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="ident">identifier search</a>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="search">freetext search</a>&nbsp;<b>]</b> ~ <b>[</b>&nbsp;<a href="find">file search</a>&nbsp;<b>]</b> ~
</div>

<hr>
This page was automatically generated by the 
<a href="http:blurb.html">LXR engine</a>.
<br>
Visit the <a href="http://lxr.linux.no/">LXR main site</a> for more
information.
</html>
