# Viterbi Algorithm analyze

## 1 Abstract

## 2 Abbreviation
| Symbol | Illustration |
|:-----------|:------------|
| k | length of information bits |
| N | length of convolution encode bits |
| conv213 | convolution code K = 1; N = 2 ; constrain length =3 |
| poly | polynomial of convolutional code |

## 3 Introduction
The Viterbi algorithm is a dynamic programming algorithm. This algorithm is used to obtaining the maximum a posteriori probability estimate of the most likely sequence of hidden states. In application, Viterbi algorithm has found universal application in decoding the convolutional codes.

## 4 算法原理说明
**note:** 该算法原理说明以 conv213 poly = [7 5] 的卷积码的维特比译码器进行说明




