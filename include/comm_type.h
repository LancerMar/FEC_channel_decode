#pragma once

namespace FEC_CHANNEL_DECODE{
    enum FEC_Obj_type{
        FEC_channel_decode_test,
        VIT,
        FEC_Obj_default,
    };

    struct Result
    {
        bool is_success = true;
    };
}
