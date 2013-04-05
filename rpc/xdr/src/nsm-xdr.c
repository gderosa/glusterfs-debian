/*
  Copyright (c) 2007-2012 Red Hat, Inc. <http://www.redhat.com>
  This file is part of GlusterFS.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nsm-xdr.h"

bool_t
xdr_sm_name (XDR *xdrs, sm_name *objp)
{
	 if (!xdr_string (xdrs, &objp->mon_name, SM_MAXSTRLEN))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_res (XDR *xdrs, res *objp)
{
	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_sm_stat_res (XDR *xdrs, sm_stat_res *objp)
{
	 if (!xdr_res (xdrs, &objp->res_stat))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_sm_stat (XDR *xdrs, sm_stat *objp)
{
	 if (!xdr_int (xdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_my_id (XDR *xdrs, my_id *objp)
{
	 if (!xdr_string (xdrs, &objp->my_name, SM_MAXSTRLEN))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->my_prog))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->my_vers))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->my_proc))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mon_id (XDR *xdrs, mon_id *objp)
{
	 if (!xdr_string (xdrs, &objp->mon_name, SM_MAXSTRLEN))
		 return FALSE;
	 if (!xdr_my_id (xdrs, &objp->my_id))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mon (XDR *xdrs, mon *objp)
{
	 if (!xdr_mon_id (xdrs, &objp->mon_id))
		 return FALSE;
	 if (!xdr_opaque (xdrs, objp->priv, 16))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nsm_callback_status (XDR *xdrs, nsm_callback_status *objp)
{
	 if (!xdr_string (xdrs, &objp->mon_name, SM_MAXSTRLEN))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->state))
		 return FALSE;
	 if (!xdr_opaque (xdrs, objp->priv, 16))
		 return FALSE;
	return TRUE;
}
