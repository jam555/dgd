# define INCLUDE_TELNET
# include "dgd.h"
# include "str.h"
# include "array.h"
# include "object.h"
# include "interpret.h"
# include "comm.h"

typedef struct _user_ {
    union {
	object *obj;		/* associated object */
	struct _user_ *next;	/* next in free list */
    } u;
    int inbufsz;		/* bytes in input buffer */
    int outbufsz;		/* bytes in output buffer */
    char state;			/* input state */
    bool echo;			/* output state */
    connection *conn;		/* connection */
    char inbuf[INBUF_SIZE];	/* input buffer */
    char outbuf[OUTBUF_SIZE];	/* output buffer */
} user;

# define TS_DATA	0
# define TS_IAC		1
# define TS_IGNORE	2

static user **users;		/* array of users */
static int maxusers;		/* max # of users */
static int nusers;		/* current # of users */
static int newlines;		/* current # of newlines in all input buffers */
static object *this_user;	/* current user */

/*
 * NAME:	comm->init()
 * DESCRIPTION:	initialize communications
 */
void comm_init(nusers, port_number)
int nusers, port_number;
{
    register int i;
    register user **usr;

    conn_init(nusers, port_number);
    users = ALLOC(user*, maxusers = nusers);
    for (i = nusers, usr = users; i > 0; --i, usr++) {
	*usr = (user *) NULL;
    }
}

/*
 * NAME:	comm->finish()
 * DESCRIPTION:	terminate connections
 */
void comm_finish()
{
    comm_flush(FALSE);
    conn_finish();
}

/*
 * NAME:	comm->new()
 * DESCRIPTION:	accept a new connection
 */
static void comm_new(obj, conn)
object *obj;
connection *conn;
{
    static char echo_on[] = { IAC, WONT, TELOPT_ECHO };
    register user **usr;

    if (obj->flags & (O_USER | O_EDITOR)) {
	fatal("user object is already in use by user or editor");
    }
    for (usr = users; *usr != (user *) NULL; usr++) ;
    mstatic();
    *usr = ALLOC(user, 1);
    mdynamic();
    (*usr)->u.obj = obj;
    obj->flags |= O_USER;
    obj->eduser = usr - users;
    (*usr)->inbufsz = 0;
    /* start with echo on */
    memcpy((*usr)->outbuf, echo_on, 3);
    (*usr)->outbufsz = 3;
    (*usr)->state = TS_DATA;
    (*usr)->echo = TRUE;
    (*usr)->conn = conn;
    nusers++;
    this_user = obj;
}

/*
 * NAME:	comm->del()
 * DESCRIPTION:	delete a connection
 */
static void comm_del(usr)
register user **usr;
{
    register char *p, *q;
    register int n;
    object *obj;

    conn_del((*usr)->conn);
    p = (*usr)->inbuf;
    n = (*usr)->inbufsz;
    while (n > 0 && (q=(char *) memchr(p, LF, n)) != (char *) NULL) {
	--newlines;
	q++;
	n -= q - p;
	p = q;
    }
    obj = (*usr)->u.obj;
    obj->flags &= ~O_USER;
    FREE(*usr);
    *usr = (user *) NULL;
    --nusers;
    if (i_call(obj, "close", TRUE, 0)) {
	i_del_value(sp++);
    }
    if (obj == this_user) {
	this_user = (object *) NULL;
    }
}

/*
 * NAME:	comm->send()
 * DESCRIPTION:	send a message to a user
 */
void comm_send(obj, str)
object *obj;
string *str;
{
    register user *usr;
    register char *p, *q;
    register unsigned short len, size;

    usr = users[UCHAR(obj->eduser)];
    size = usr->outbufsz;
    q = usr->outbuf + size;
    for (p = str->text, len = str->len; len != 0; p++, --len) {
	if (UCHAR(*p) == IAC) {
	    /*
	     * double the telnet IAC character
	     */
	    if (size >= OUTBUF_SIZE - 1) {
		conn_write(usr->conn, q = usr->outbuf, size);
		size = 0;
	    }
	    *q++ = IAC;
	    size++;
	} else if (*p == LF) {
	    /*
	     * insert CR before LF
	     */
	    if (size >= OUTBUF_SIZE - 1) {
		conn_write(usr->conn, q = usr->outbuf, size);
		size = 0;
	    }
	    *q++ = CR;
	    size++;
	} else if ((*p & 0x7f) < ' ' && *p != HT && *p != BEL && *p != BS) {
	    /*
	     * illegal character
	     */
	    continue;
	}
	if (size == OUTBUF_SIZE) {
	    conn_write(usr->conn, q = usr->outbuf, size);
	    size = 0;
	}
	*q++ = *p;
	size++;
    }
    usr->outbufsz = size;
}

/*
 * NAME:	comm->echo()
 * DESCRIPTION:	turn on/off input echoing for a user
 */
void comm_echo(obj, echo)
object *obj;
bool echo;
{
    register user *usr;
    char buf[3];

    usr = users[UCHAR(obj->eduser)];
    if (echo != usr->echo) {
	buf[0] = IAC;
	buf[1] = (echo) ? WONT : WILL;
	buf[2] = TELOPT_ECHO;
	conn_write(usr->conn, buf, 3);
	usr->echo = echo;
    }
}

/*
 * NAME:	comm->flush()
 * DESCRIPTION:	flush output to all users
 */
void comm_flush(prompt)
bool prompt;
{
    register user **usr;
    register int i, size;
    register char *p;

    for (usr = users, i = maxusers; i > 0; usr++, --i) {
	if (*usr != (user *) NULL && (size=(*usr)->outbufsz) > 0) {
	    if (prompt && (*usr)->u.obj == this_user) {
		/*
		 * append "go ahead" to indicate that the prompt has been sent
		 */
		if (size >= OUTBUF_SIZE - 1) {
		    conn_write((*usr)->conn, (*usr)->outbuf, size);
		    size = 0;
		}
		p = (*usr)->outbuf + size;
		*p++ = IAC;
		*p++ = GA;
		size += 2;
	    }
	    conn_write((*usr)->conn, (*usr)->outbuf, size);
	    (*usr)->outbufsz = 0;
	}
    }
    this_user = (object *) NULL;
}

/*
 * NAME:	comm->receive()
 * DESCRIPTION:	receive a message from a user
 */
object *comm_receive(buf, size)
char *buf;
int *size;
{
    static int lastuser;
    register int n, i, state;
    register char *p, *q;

    if (nusers < maxusers) {
	connection *conn;
	object *o;

	/*
	 * accept new connection
	 */
	conn = conn_new();
	if (conn != (connection *) NULL) {
	    if (ec_push()) {
		conn_del(conn);		/* delete connection */
		error((char *) NULL);	/* pass on error */
	    }
	    call_driver_object("connect", 0);
	    ec_pop();
	    if (sp->type != T_OBJECT) {
		fatal("driver->connect() did not return an object");
	    }
	    comm_new(o = o_object(sp->oindex, sp->u.objcnt), conn);
	    sp++;
	    if (i_call(o, "open", TRUE, 0)) {
		i_del_value(sp++);
	    }
	    comm_flush(TRUE);
	}
    }

    /*
     * read input from users
     */
    this_user = (object *) NULL;
    n = conn_select(newlines == 0);
    if (n <= 0) {
	/*
	 * new user, call_out to do, or timeout
	 */
	if (newlines == 0) {
	    return (object *) NULL;
	}
    } else {
	static char intr[] = { '\177', IAC, WILL, TELOPT_TM };
	static char brk[] = { '\034', IAC, WILL, TELOPT_TM };
	register user **usr;

	for (i = maxusers, usr = users; i > 0; --i, usr++) {
	    if (*usr != (user *) NULL && (*usr)->inbufsz != INBUF_SIZE) {
		p = q = (*usr)->inbuf + (*usr)->inbufsz;
		n = conn_read((*usr)->conn, p, INBUF_SIZE - (*usr)->inbufsz);
		if (n < 0) {
		    comm_del(usr);
		    continue;
		}
		state = (*usr)->state;
		while (n > 0) {
		    switch (state) {
		    case TS_DATA:
			switch (UCHAR(*p)) {
			case IAC:
			    state = TS_IAC;
			    break;

			case BS:
			case 0x7f:
			    if (q != (*usr)->inbuf && q[-1] != LF) {
				--q;
			    }
			    break;

			case LF:
			    if (q != (*usr)->inbuf && q[-1] == CR) {
				--q;
			    }
			    newlines++;
			default:
			    *q++ = *p;
			}
			break;

		    case TS_IAC:
			switch (UCHAR(*p)) {
			case IAC:
			    *q++ = *p;
			    break;

			case WILL:
			case WONT:
			case DO:
			case DONT:
			    state = TS_IGNORE;
			    break;

			case IP:
			    conn_write((*usr)->conn, intr, 4);
			    break;

			case BREAK:
			    conn_write((*usr)->conn, brk, 4);
			    break;

			default:
			    state = TS_DATA;
			    break;
			}
			break;

		    case TS_IGNORE:
			state = TS_DATA;
			break;
		    }
		    p++;
		    --n;
		}
		(*usr)->state = state;
		(*usr)->inbufsz = q - (*usr)->inbuf;
	    }
	}
    }

    if (newlines != 0) {
	register user *usr;

	n = lastuser;
	for (;;) {
	    n = (n + 1) % maxusers;
	    usr = users[n];
	    if (usr != (user *) NULL && usr->inbufsz != 0) {
		p = (char *) memchr(usr->inbuf, LF, usr->inbufsz);
		if (p != (char *) NULL) {
		    /*
		     * input terminated by \n
		     */
		    --newlines;
		    lastuser = n;
		    *size = n = p - usr->inbuf;
		    if (n != 0) {
			memcpy(buf, usr->inbuf, n);
		    }
		    n++;	/* skip \n */
		    usr->inbufsz -= n;
		    if (usr->inbufsz != 0) {
			/* can't rely on memcpy */
			for (p = usr->inbuf, q = p + n, n = usr->inbufsz; n > 0;
			     --n) {
			    *p++ = *q++;
			}
		    }
		    return this_user = usr->u.obj;
		} else if (usr->inbufsz == INBUF_SIZE) {
		    /*
		     * input buffer full
		     */
		    lastuser = n;
		    memcpy(buf, usr->inbuf, *size = INBUF_SIZE);
		    usr->inbufsz = 0;
		    return this_user = usr->u.obj;
		}
	    }
	    if (n == lastuser) {
		return (object *) NULL;
	    }
	}
    }

    return (object *) NULL;
}

/*
 * NAME:	comm->ip_number()
 * DESCRIPTION:	return the ip number of a user (as a string)
 */
string *comm_ip_number(obj)
object *obj;
{
    char *ipnum;

    ipnum = conn_ipnum(users[UCHAR(obj->eduser)]->conn);
    return str_new(ipnum, (long) strlen(ipnum));
}

/*
 * NAME:	comm->close()
 * DESCRIPTION:	remove a user
 */
void comm_close(obj)
object *obj;
{
    register user **usr;

    usr = &users[UCHAR(obj->eduser)];
    if ((*usr)->outbufsz != 0) {
	/*
	 * flush last bit of output
	 */
	conn_write((*usr)->conn, (*usr)->outbuf, (*usr)->outbufsz);
    }
    comm_del(usr);
}

/*
 * NAME:	comm->user()
 * DESCRIPTION:	return the current user
 */
object *comm_user()
{
    return this_user;
}

/*
 * NAME:	comm->users()
 * DESCRIPTION:	return an array with all user objects
 */
array *comm_users()
{
    array *a;
    register int i;
    register user **usr;
    register value *v;

    a = arr_new((long) nusers);
    v = a->elts;
    for (i = nusers, usr = users; i > 0; usr++) {
	if (*usr != (user *) NULL) {
	    v->type = T_OBJECT;
	    v->oindex = (*usr)->u.obj->index;
	    v->u.objcnt = (*usr)->u.obj->count;
	    v++;
	    --i;
	}
    }
    return a;
}
