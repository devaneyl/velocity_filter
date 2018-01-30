/* velocity_filter.c
  eq_event_this alsa sound filter is a hack to get around the Korg Volca FM ignoring
  note velocity parameters over midi.  As note events are received, it will 
  read the note velocity and create new system velocity events, sending them 
  before the note.

*/

#include <alsa/asoundlib.h>

void velocity_filter(snd_seq_t *seq, int port, snd_seq_event_t *ev, snd_seq_event_t *vel_ev)
{
    /* Set up basic parameters on the system velocity event */
    vel_ev->data.control.channel = 0; 
    vel_ev->type = SND_SEQ_EVENT_CONTROLLER;
    vel_ev->data.control.param = 0x29;   /* type 0x29 is velocity */
    vel_ev->data.control.value = 0;      /* set to nothing for starts */
    
   
    while (snd_seq_event_input(seq, &ev) >= 0)
    {
	if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity != 0)
        
        /* load the keyboard velocity over into our new event */
        vel_ev->data.control.value = ev->data.note.velocity;

        /* dump the event */
	snd_seq_ev_set_source(vel_ev, port);
	snd_seq_ev_set_subs(vel_ev);
	snd_seq_ev_set_direct(vel_ev);
	snd_seq_event_output(seq, vel_ev);
	snd_seq_drain_output(seq);

        /* dump the original note on event */
	snd_seq_ev_set_source(ev, port);
	snd_seq_ev_set_subs(ev);
	snd_seq_ev_set_direct(ev);
	snd_seq_event_output(seq, ev);
	snd_seq_drain_output(seq);
    }
}

snd_seq_t *open_client()
{
    snd_seq_t *handle;
    int err;
    err = snd_seq_open(&handle, "default", SND_SEQ_OPEN_DUPLEX, 0);
    if (err <0)
    {
	fprintf(stderr, "Error opening sequencer\n");
	exit(1);
    }
    snd_seq_set_client_name(handle, "Velocity Filter");
    return handle;
}

int create_port(snd_seq_t *seq,char *name)
{
    int portid;
    if ((portid = snd_seq_create_simple_port(seq, name, 
		    SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE|
		    SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
		    SND_SEQ_PORT_TYPE_APPLICATION)) < 0) 
    {
	fprintf(stderr, "Error creating port %s\n", name);
	exit(1);
    }
    return portid;
}

int main(void)
{
    snd_seq_t *seq;
    snd_seq_event_t ev;
    snd_seq_event_t vel_ev;
    int port;

    seq = open_client();
    port = create_port(seq,"Velocity");
    velocity_filter(seq, port, &ev, &vel_ev);
}
