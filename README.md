# FEC_channel_decode
We build this project for everyone can simply understand and use FEC decoder to process signal.We provide various FEC(Forward error code) decoders for different FEC encoders.
1. viterbi decoder 


| FEC decode algorithm | FEC encode type | performance |detail |
|:-----------|:------------|:-----------|:-----------|
| Viterbi | convolution code | | [vit_decode](DOC/vit_decode_analyse.md)  |
| MIN_SUM | LDPC | [min_sum_decoder](DOC/LDPC/min_sum_deocder_performance_analyse.md) |[]()|
| MAX_Log_MAP | Turbo | []() |[]()|

# Compile
    This project supports multi platform compilation,include  
# Linux

```bash
mkdir build
cd build
cmake ..
```

# Windows

```bash
mkdir build
cd build
cmake ..
```

