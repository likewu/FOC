#include "CanTp_PBcfg.h"

        //{%- if channel.receivers is defined %}
#ifndef CANTP_N_SA_1

#define CANTP_N_SA_1 (0x01)

#define CanTp_START_SEC_CONST_8
#include "CanTp_MemMap.h"

static const CanTp_NSaType CanTp_NSa1 = {
    CANTP_N_SA_1
};

#define CanTp_STOP_SEC_CONST_8

#endif /* #ifndef CANTP_N_SA_{{'%02X' % receiver.source_address}} */

#ifndef CANTP_N_TA_1

#define CANTP_N_TA_1 (0x01)

static const CanTp_NTaType CanTp_NTa1 = {
    CANTP_N_TA_1
};

#endif /* #ifndef CANTP_N_TA_{{'%02X' % receiver.target_address}} */

#ifndef CANTP_N_AE_1

#define CANTP_N_AE_1 (0x01)

static const CanTp_NAeType CanTp_NAe1 = {
    CANTP_N_AE_1
};

#endif /* #ifndef CANTP_N_AE_{{'%02X' % receiver.address_extension}} */
        //{% endif -%}


        //{%- if channel.receivers is defined %}
#define CanTp_START_SEC_CONST_UNSPECIFIED

static const CanTp_RxNSduType CanTp_RxNSduConfig1Channel1[] = {
    {
        0x01,
        &CanTp_NSa1,
        &CanTp_NTa1,
        &CanTp_NAe1,
        0x01,
        (uint32)((ieee_float)1000 * (ieee_float)1000000),
        (uint32)((ieee_float)1000 * (ieee_float)1000000),
        (uint32)((ieee_float)1000 * (ieee_float)1000000),
        0x0001,
        (uint32)((ieee_float)1000 * (ieee_float)1000000),
        1,
        CANTP_ON,
        1,
        0x0001
    }
};

#define CanTp_STOP_SEC_CONST_UNSPECIFIED
        //{%- endif %}

static const CanTp_ChannelType CanTp_ChannelConfig1[] = {
    {
        {
            &CanTp_RxNSduConfig1Channel1[0x00u],
            sizeof(CanTp_RxNSduConfig1Channel1) / sizeof(CanTp_RxNSduConfig1Channel1[0x00u]),
 
            NULL_PTR,
            0x00u
        }
    }
};

const CanTp_ConfigType CanTp_Config[1] =
{
    {
        ((uint32)((ieee_float)1000 * (ieee_float)1000000)),
        sizeof(CanTp_ChannelConfig1) / sizeof(CanTp_ChannelConfig1[0x00u]),
        &CanTp_ChannelConfig1[0x00u],
        1,
    },
};
