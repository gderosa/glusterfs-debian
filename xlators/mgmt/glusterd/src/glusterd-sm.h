/*
  Copyright (c) 2006-2010 Gluster, Inc. <http://www.gluster.com>
  This file is part of GlusterFS.

  GlusterFS is free software; you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation; either version 3 of the License,
  or (at your option) any later version.

  GlusterFS is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#ifndef _GLUSTERD_SM_H_
#define _GLUSTERD_SM_H_

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include "uuid.h"

#include "rpc-clnt.h"
#include "glusterfs.h"
#include "xlator.h"
#include "logging.h"
#include "call-stub.h"
#include "fd.h"
#include "byte-order.h"
//#include "glusterd.h"
#include "rpcsvc.h"

struct glusterd_store_handle_ {
        char    *path;
        int     fd;
        FILE    *read;
};

typedef struct glusterd_store_handle_  glusterd_store_handle_t;

typedef enum glusterd_friend_sm_state_ {
        GD_FRIEND_STATE_DEFAULT = 0,
        GD_FRIEND_STATE_REQ_SENT,
        GD_FRIEND_STATE_REQ_RCVD,
        GD_FRIEND_STATE_BEFRIENDED,
        GD_FRIEND_STATE_REQ_ACCEPTED,
        GD_FRIEND_STATE_REQ_SENT_RCVD,
        GD_FRIEND_STATE_REJECTED,
        GD_FRIEND_STATE_UNFRIEND_SENT,
        GD_FRIEND_STATE_MAX
} glusterd_friend_sm_state_t;

typedef struct glusterd_peer_state_info_ {
        glusterd_friend_sm_state_t   state;
        struct timeval          transition_time;
}glusterd_peer_state_info_t;

typedef struct glusterd_peer_hostname_ {
        char                    *hostname;
        struct list_head        hostname_list;
}glusterd_peer_hostname_t;

typedef struct glusterd_sm_transition_ {
        int             old_state;
        int             event;
        int             new_state;
        time_t          time;
} glusterd_sm_transition_t;

typedef struct glusterd_sm_tr_log_ {
        glusterd_sm_transition_t    *transitions;
        size_t                      current;
        size_t                      size;
        size_t                      count;
        char*                       (*state_name_get) (int);
        char*                       (*event_name_get) (int);
} glusterd_sm_tr_log_t;

struct glusterd_peerinfo_ {
        uuid_t                          uuid;
        char                            uuid_str[50];
        glusterd_peer_state_info_t      state;
        char                            *hostname;
        int                             port;
        struct list_head                uuid_list;
        struct list_head                op_peers_list;
        struct rpc_clnt                 *rpc;
        int                             connected;
        glusterd_store_handle_t         *shandle;
        glusterd_sm_tr_log_t            sm_log;
};

typedef struct glusterd_peerinfo_ glusterd_peerinfo_t;

typedef enum glusterd_ev_gen_mode_ {
        GD_MODE_OFF,
        GD_MODE_ON,
        GD_MODE_SWITCH_ON
} glusterd_ev_gen_mode_t;

typedef struct glusterd_peer_ctx_args_ {
        rpcsvc_request_t        *req;
        glusterd_ev_gen_mode_t  mode;
} glusterd_peerctx_args_t;

typedef struct glusterd_peer_ctx_ {
        glusterd_peerctx_args_t        args;
        glusterd_peerinfo_t            *peerinfo;
} glusterd_peerctx_t;

typedef enum glusterd_friend_sm_event_type_ {
        GD_FRIEND_EVENT_NONE = 0,
        GD_FRIEND_EVENT_PROBE,
        GD_FRIEND_EVENT_INIT_FRIEND_REQ,
        GD_FRIEND_EVENT_RCVD_ACC,
        GD_FRIEND_EVENT_LOCAL_ACC,
        GD_FRIEND_EVENT_RCVD_RJT,
        GD_FRIEND_EVENT_LOCAL_RJT,
        GD_FRIEND_EVENT_RCVD_FRIEND_REQ,
        GD_FRIEND_EVENT_INIT_REMOVE_FRIEND,
        GD_FRIEND_EVENT_RCVD_REMOVE_FRIEND,
        GD_FRIEND_EVENT_REMOVE_FRIEND,
        GD_FRIEND_EVENT_CONNECTED,
        GD_FRIEND_EVENT_MAX
} glusterd_friend_sm_event_type_t;


typedef enum glusterd_friend_update_op_ {
        GD_FRIEND_UPDATE_NONE = 0,
        GD_FRIEND_UPDATE_ADD,
        GD_FRIEND_UPDATE_DEL,
} glusterd_friend_update_op_t;


struct glusterd_friend_sm_event_ {
        struct list_head        list;
        glusterd_peerinfo_t     *peerinfo;
        void                    *ctx;
        glusterd_friend_sm_event_type_t event;
};

typedef struct glusterd_friend_sm_event_ glusterd_friend_sm_event_t;

typedef int (*glusterd_friend_sm_ac_fn) (glusterd_friend_sm_event_t *, void *);

typedef struct glusterd_sm_ {
        glusterd_friend_sm_state_t      next_state;
        glusterd_friend_sm_ac_fn        handler;
} glusterd_sm_t;

typedef struct glusterd_friend_req_ctx_ {
        uuid_t                  uuid;
        char                    *hostname;
        rpcsvc_request_t        *req;
        int                      port;
        dict_t                  *vols;
} glusterd_friend_req_ctx_t;

typedef struct glusterd_friend_update_ctx_ {
        uuid_t                  uuid;
        char                    *hostname;
        int                     op;
} glusterd_friend_update_ctx_t;

typedef struct glusterd_probe_ctx_ {
        char                    *hostname;
        rpcsvc_request_t        *req;
        int                      port;
} glusterd_probe_ctx_t;
int
glusterd_friend_sm_new_event (glusterd_friend_sm_event_type_t event_type,
                              glusterd_friend_sm_event_t **new_event);
int
glusterd_friend_sm_inject_event (glusterd_friend_sm_event_t *event);

int
glusterd_friend_sm_init ();

int
glusterd_friend_sm ();

void
glusterd_destroy_probe_ctx (glusterd_probe_ctx_t *ctx);

void
glusterd_destroy_friend_req_ctx (glusterd_friend_req_ctx_t *ctx);

char*
glusterd_friend_sm_state_name_get (int state);

char*
glusterd_friend_sm_event_name_get (int event);

int
glusterd_broadcast_friend_delete (char *hostname, uuid_t uuid);
void
glusterd_destroy_friend_update_ctx (glusterd_friend_update_ctx_t *ctx);
#endif