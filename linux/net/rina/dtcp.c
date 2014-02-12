/*
 * DTCP (Data Transfer Control Protocol)
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define RINA_PREFIX "dtcp"

#include "logs.h"
#include "utils.h"
#include "debug.h"
#include "dtcp.h"
#include "rmt.h"
#include "connection.h"

/* This is the DT-SV part maintained by DTCP */
struct dtcp_sv {
        /* Control state */
        uint_t       max_pdu_size;

        /* TimeOuts */

        /*
         * Time interval sender waits for a positive ack before
         * retransmitting
         */
        timeout_t    trd;

        /*
         * When flow control is rate based this timeout may be
         * used to pace number of PDUs sent in TimeUnit
         */
        uint_t       pdus_per_time_unit;

        /* Sequencing */

        /*
         * Outbound: NextSndCtlSeq contains the Sequence Number to
         * be assigned to a control PDU
         */
        seq_num_t    next_snd_ctl_seq;

        /*
         * Inbound: LastRcvCtlSeq - Sequence number of the next
         * expected // Transfer(? seems an error in the spec’s
         * doc should be Control) PDU received on this connection
         */
        seq_num_t    last_rcv_ctl_seq;

        /*
         * Retransmission: There’s no retransmission queue,
         * when a lost PDU is detected a new one is generated
         */

        /* Outbound */
        seq_num_t    last_snd_data_ack;

        /*
         * Seq number of the lowest seq number expected to be
         * Acked. Seq number of the first PDU on the
         * RetransmissionQ.
         */
        seq_num_t    send_left_wind_edge;

        /*
         * Maximum number of retransmissions of PDUs without a
         * positive ack before declaring an error
         */
        uint_t       data_retransmit_max;

        /* Inbound */
        seq_num_t    last_rcv_data_ack;
        seq_num_t    rcv_left_wind_edge;

        /* Time (ms) over which the rate is computed */
        uint_t       time_unit;

        /* Flow Control State */

        /* Outbound */
        uint_t       sndr_credit;

        /* snd_rt_wind_edge = LastSendDataAck + PDU(credit) */
        seq_num_t    snd_rt_wind_edge;

        /* PDUs per TimeUnit */
        uint_t       sndr_rate;

        /* PDUs already sent in this time unit */
        uint_t       pdus_sent_in_time_unit;

        /* Inbound */

        /*
         * PDUs receiver believes sender may send before extending
         * credit or stopping the flow on the connection
         */
        uint_t       rcvr_credit;

        /* Value of credit in this flow */
        seq_num_t    rcvr_rt_wind_edge;

        /*
         * Current rate receiver has told sender it may send PDUs
         * at.
         */
        uint_t       rcvr_rate;

        /*
         * PDUs received in this time unit. When it equals
         * rcvr_rate, receiver is allowed to discard any PDUs
         * received until a new time unit begins
         */
        uint_t       pdus_rcvd_in_time_unit;

        bool         set_drf_flag;
};

struct dtcp_policies {
        int (* flow_init)(struct dtcp * instance);
        int (* sv_update)(struct dtcp * instance, seq_num_t seq);
        int (* lost_control_pdu)(struct dtcp * instance);
        int (* rtt_estimator)(struct dtcp * instance);
        int (* retransmission_timer_expiry)(struct dtcp * instance);
        int (* received_retransmission)(struct dtcp * instance);
        int (* rcvr_ack)(struct dtcp * instance);
        int (* sending_ack)(struct dtcp * instance);
        int (* sending_ack_list)(struct dtcp * instance);
        int (* initial_credit)(struct dtcp * instance);
        int (* initial_rate)(struct dtcp * instance);
        int (* receiving_flow_control)(struct dtcp * instance);
        int (* update_credit)(struct dtcp * instance);
        int (* flow_control_overrun)(struct dtcp * instance);
        int (* reconcile_flow_conflict)(struct dtcp * instance);
        int (* rcvr_flow_control)(struct dtcp * instance, seq_num_t seq);
};

struct dtcp {
        struct dt *            parent;

        /*
         * NOTE: The DTCP State Vector can be discarded during long periods of
         *       no traffic
         */
        struct dtcp_sv *       sv; /* The state-vector */
        struct dtcp_policies * policies;
        struct connection *    conn;
        struct rmt *           rmt;

        /* FIXME: Add QUEUE(flow_control_queue, pdu) */
        /* FIXME: Add QUEUE(closed_window_queue, pdu) */
        /* FIXME: Add QUEUE(rx_control_queue, ...) */
};

int dtcp_common_rcv_control(struct dtcp * dtcp, struct pdu * pdu)
{
        struct pci * pci;
        pdu_type_t   type;

        pci = pdu_pci_get_ro(pdu);
        if (!pci)
                return -1;

        type = pci_type(pci);

        if (!pdu_type_is_control(type)) {
                LOG_ERR("CommonRCVControl policy received a non-control PDU!");
                return -1;
        }
        ASSERT(pdu_type_is_control(type));

        /*
        * FIXME: missing steps described in the specs
        * 1- Retrieve the time of this Ack and calculate the RTT with
        * RTTEstimator policy
        */

        /* IF it is FlowControl Only */
        if (type == PDU_TYPE_FC) {
                dtcp->sv->
        }
        return 0;
}

static int default_rcvr_flow_control(struct dtcp * dtcp, seq_num_t seq)
{
        struct pdu * pdu_ctrl;
        struct pci * pci;

        pdu_ctrl = pdu_create();
        if (!pdu_ctrl)
                return -1;

        pci = pci_create();
        if (!pci) {
                pdu_destroy(pdu_ctrl);
                return -1;
        }
        if (pci_format(pci,
                       0,
                       0,
                       0,
                       0,
                       dtcp->sv->next_snd_ctl_seq,
                       0,
                       PDU_TYPE_ACK_AND_FC)) {
                pdu_destroy(pdu_ctrl);
                pci_destroy(pci);
                return -1;
        }

        if (pdu_pci_set(pdu_ctrl, pci)) {
                pdu_destroy(pdu_ctrl);
                pci_destroy(pci);
                return -1;
        }

        pdu_control_ack_flow(pdu_ctrl,
                             dtcp->sv->last_rcv_ctl_seq,
                             seq,
                             seq + dtcp->sv->rcvr_credit,
                             0,
                             1,
                             dtcp->sv->send_left_wind_edge,
                             dtcp->sv->snd_rt_wind_edge,
                             dtcp->sv->sndr_rate);

        rmt_send(dtcp->rmt, 0, 0, pdu_ctrl);

        return -1;
}


/* FIXME: Mock up code */
static int default_sv_update(struct dtcp * dtcp, seq_num_t seq)
{
        LOG_MISSING;

        if (!dtcp)
                return -1;

        /*FIXME: here it goes rcvr_flow_control_policy */

        if (dtcp->conn->policies_params.flow_ctrl &&
            !dtcp->conn->policies_params.rtx_ctrl) {
                return dtcp->policies->receiving_flow_control(dtcp);
        }

        return -1;
}

static struct dtcp_sv default_sv = {
        .max_pdu_size           = 0,
        .trd                    = 0,
        .pdus_per_time_unit     = 0,
        .next_snd_ctl_seq       = 0,
        .last_rcv_ctl_seq       = 0,
        .last_snd_data_ack      = 0,
        .send_left_wind_edge    = 0,
        .data_retransmit_max    = 0,
        .last_rcv_data_ack      = 0,
        .rcv_left_wind_edge     = 0,
        .time_unit              = 0,
        .sndr_credit            = 0,
        .snd_rt_wind_edge       = 0,
        .sndr_rate              = 0,
        .pdus_sent_in_time_unit = 0,
        .rcvr_credit            = 0,
        .rcvr_rt_wind_edge      = 0,
        .rcvr_rate              = 0,
        .pdus_rcvd_in_time_unit = 0,
        .set_drf_flag           = false,
};

static struct dtcp_policies default_policies = {
        .flow_init                   = NULL,
        .sv_update                   = default_sv_update,
        .lost_control_pdu            = NULL,
        .rtt_estimator               = NULL,
        .retransmission_timer_expiry = NULL,
        .received_retransmission     = NULL,
        .sending_ack                 = NULL,
        .sending_ack_list            = NULL,
        .initial_credit              = NULL,
        .initial_rate                = NULL,
        .receiving_flow_control      = NULL,
        .update_credit               = NULL,
        .flow_control_overrun        = NULL,
        .reconcile_flow_conflict     = NULL,
        .rcvr_ack                    = NULL,
        .rcvr_flow_control           = default_rcvr_flow_control,
};

struct dtcp * dtcp_create(struct dt *         dt,
                          struct connection * conn,
                          struct rmt *        rmt)
{
        struct dtcp * tmp;

        if (!dt) {
                LOG_ERR("No DT passed, bailing out");
                return NULL;
        }

        tmp = rkzalloc(sizeof(*tmp), GFP_KERNEL);
        if (!tmp) {
                LOG_ERR("Cannot create DTCP state-vector");
                return NULL;
        }

        tmp->parent = dt;

        tmp->sv = rkzalloc(sizeof(*tmp->sv), GFP_KERNEL);
        if (!tmp->sv) {
                LOG_ERR("Cannot create DTCP state-vector");
                dtcp_destroy(tmp);
                return NULL;
        }
        tmp->policies = rkzalloc(sizeof(*tmp->policies), GFP_KERNEL);
        if (!tmp->policies) {
                LOG_ERR("Cannot create DTCP policies");
                dtcp_destroy(tmp);
                return NULL;
        }

        *tmp->sv       = default_sv;
        /* FIXME: fixups to the state-vector should be placed here */

        *tmp->policies = default_policies;
        /* FIXME: fixups to the policies should be placed here */

        tmp->conn      = conn;

        LOG_DBG("Instance %pK created successfully", tmp);

        return tmp;
}

int dtcp_destroy(struct dtcp * instance)
{
        if (!instance) {
                LOG_ERR("Bad instance passed, bailing out");
                return -1;
        }

        if (instance->sv)       rkfree(instance->sv);
        if (instance->policies) rkfree(instance->policies);
        rkfree(instance);

        LOG_DBG("Instance %pK destroyed successfully", instance);

        return 0;
}

int dtcp_send(struct dtcp * instance,
              struct sdu *  sdu)
{
        LOG_MISSING;

        /* Takes the pdu and enqueue in its internal queues */

        return -1;
}

int dtcp_sv_update(struct dtcp * instance,
                   seq_num_t     seq)
{
        if (!instance) {
                LOG_ERR("Bogus instance passed");
                return -1;
        }

        instance->sv->send_left_wind_edge = seq;

        return 0;
}

