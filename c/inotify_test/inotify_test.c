#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include <signal.h>

int keep_running = 1;
static int watched_items;

struct queue_entry {
  struct queue_entry * next_ptr;
  struct inotify_event inot_ev;
};

typedef struct queue_entry * queue_entry_t;

struct queue_struct {
  struct queue_entry * head;
  struct queue_entry * tail;
};

typedef struct queue_struct *queue_t;

void signal_handler (int signum) { 
  keep_running = 0;
}

int open_inotify_fd() {
  int fd;

  watched_items = 0;
  fd = inotify_init ();

  if (fd < 0) {
    perror ("inotify_init () = ");
  }
  return fd;
}

int watch_dir (int fd, const char *dirname, unsigned long mask) {
  int wd;
  wd = inotify_add_watch (fd, dirname, mask);
  if (wd < 0) {
    printf ("Cannot add watch for \"%s\" with event mask %lX", dirname, mask);
    fflush (stdout);
    perror (" ");
  } else {
    watched_items++;
    printf ("Watching %s WD=%d\n", dirname, wd);
    printf ("Watching = %d items\n", watched_items);
  }
  return wd;
}

int process_inotify_events (queue_t q, int fd) {
  while (keep_running && (watched_items > 0)) {
    if (event_check (fd) > 0) {
      int r;
      r = read_events (q, fd);
      if (r < 0) {
        break;
      } else {
        handle_events (q);
      }
    }
  }
  return 0;
}

int event_check (int fd) {
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  return select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
}

int read_events (queue_t q, int fd) {
  char buffer[16384];
  size_t buffer_i;
  struct inotify_event *pevent;
  queue_entry_t event;
  size_t r;
  size_t event_size, q_event_size;
  int count = 0;

  r = read (fd, buffer, 16384);
  if (r <= 0) return r;
  buffer_i = 0;
  while (buffer_i < r) {
    pevent = (struct inotify_event *) &buffer[buffer_i];
    event_size =  offsetof (struct inotify_event, name) + pevent->len;
    q_event_size = offsetof (struct queue_entry, inot_ev.name) + pevent->len;
    event = malloc (q_event_size);
    memmove (&(event->inot_ev), pevent, event_size);
    queue_enqueue (event, q);
    buffer_i += event_size;
    count++;
  }
  printf ("\n%d events queued\n", count);
  return count;
}

queue_entry_t  queue_dequeue (queue_t q) {
  queue_entry_t first = q->head;
  if (first) {
    q->head = first->next_ptr;
    if (q->head == NULL) {
      q->tail = NULL;
    }
    first->next_ptr = NULL;
  }
  return first;
}

void handle_event (queue_entry_t event) {
  /* If the event was associated with a filename, we will store it here */
  char *cur_event_filename = NULL;
  char *cur_event_file_or_dir = NULL;
  /* This is the watch descriptor the event occurred on */
  int cur_event_wd = event->inot_ev.wd;
  int cur_event_cookie = event->inot_ev.cookie;
  unsigned long flags;

  if (event->inot_ev.len) {
    cur_event_filename = event->inot_ev.name;
  }
  if ( event->inot_ev.mask & IN_ISDIR ) {
    cur_event_file_or_dir = "Dir";
  } else {
    cur_event_file_or_dir = "File";
  }
  flags = event->inot_ev.mask & 
    ~(IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED );

  /* Perform event dependent handler routines */
  /* The mask is the magic that tells us what file operation occurred */
  switch (event->inot_ev.mask & 
	  (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED))
  {
    case IN_ACCESS:
      printf ("ACCESS: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_MODIFY:
      printf ("MODIFY: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_ATTRIB:
      printf ("ATTRIB: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_CLOSE_WRITE:
      printf ("CLOSE_WRITE: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_CLOSE_NOWRITE:
      printf ("CLOSE_NOWRITE: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_OPEN:
      printf ("OPEN: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_MOVED_FROM:
      printf ("MOVED_FROM: %s \"%s\" on WD #%i. Cookie=%d\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd, 
              cur_event_cookie);
      break;

    case IN_MOVED_TO:
      printf ("MOVED_TO: %s \"%s\" on WD #%i. Cookie=%d\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd, 
              cur_event_cookie);
      break;

    case IN_DELETE:
      printf ("DELETE: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_CREATE:
      printf ("CREATE: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      
      break;

    case IN_DELETE_SELF:
      printf ("DELETE_SELF: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_MOVE_SELF:
      printf ("MOVE_SELF: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_UNMOUNT:
      printf ("UNMOUNT: %s \"%s\" on WD #%i\n",
	      cur_event_file_or_dir, cur_event_filename, cur_event_wd);
      break;

    case IN_Q_OVERFLOW:
      printf ("Warning: AN OVERFLOW EVENT OCCURRED: \n");
      break;

    case IN_IGNORED:
      watched_items--;
      printf ("IGNORED: WD #%d\n", cur_event_wd);
      printf("Watching = %d items\n",watched_items); 
      break;

    default:
      printf ("UNKNOWN EVENT \"%X\" OCCURRED for file \"%s\" on WD #%i\n",
	      event->inot_ev.mask, cur_event_filename, cur_event_wd);
      break;
    }

  if (flags & (~IN_ISDIR)) {
    flags = event->inot_ev.mask;
    printf ("Flags=%lX\n", flags);
  }
}

void handle_events (queue_t q) {
  queue_entry_t event;
  while (!queue_empty (q)) {
    event = queue_dequeue (q);
    handle_event (event);
    free (event);
  }
}

int queue_empty (queue_t q) {
  return q->head == NULL;
}

queue_t queue_create () {
  queue_t q;
  q = malloc (sizeof (struct queue_struct));
  if (q == NULL) exit(-1);
  q->head = q->tail = NULL;
  return q;
}

void queue_destroy (queue_t q) {
  if (q != NULL) {
    while (q->head != NULL) {
      queue_entry_t next = q->head;
      q->head = next->next_ptr;
      next->next_ptr = NULL;
      free (next);
    }
    q->head = q->tail = NULL;
    free(q);
  }
}



void queue_enqueue (queue_entry_t d, queue_t q) {
  d->next_ptr = NULL;
  if (q->tail) {
    q->tail->next_ptr = d;
    q->tail = d;
  } else {
    q->head = q->tail = d;
  }
}

int close_inotify_fd (int fd) {
  int r;

  if ((r = close (fd)) < 0) {
    perror ("close (fd) = ");
  }

  watched_items = 0;
  return r;
}

int main(int argc, char **argv) {
  int inotify_fd;
  if (signal (SIGINT, signal_handler) == SIG_IGN) {
    signal (SIGINT, SIG_IGN);
  }
  inotify_fd = open_inotify_fd ();
  if (inotify_fd > 0) {
    queue_t q;
    q = queue_create(128);
    int wd;
    int index;
    wd = 0;
    printf("\n");
    for (index = 1; (index < argc) && (wd >= 0); index++) {
      wd = watch_dir (inotify_fd, argv[index], IN_ALL_EVENTS);
    }
    if (wd > 0) {
      process_inotify_events (q, inotify_fd);
    }
    printf ("\nTerminating\n");
    close_inotify_fd (inotify_fd);
    queue_destroy (q);
  }
  return 0;
}
