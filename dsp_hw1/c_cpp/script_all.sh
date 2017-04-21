#!/bin/bash
for i in $(seq 1 10)
do
  if [ "${i}" == 1 ]; then
    ./train 1 ../model_init2.txt ../seq_model_01.txt ../model2_01.txt
    ./train 1 ../model_init2.txt ../seq_model_02.txt ../model2_02.txt
    ./train 1 ../model_init2.txt ../seq_model_03.txt ../model2_03.txt
    ./train 1 ../model_init2.txt ../seq_model_04.txt ../model2_04.txt
    ./train 1 ../model_init2.txt ../seq_model_05.txt ../model2_05.txt
    ./test modellist2.txt ../testing_data1.txt ../result2.txt
    ./a.out ../result2.txt ../score2.txt
  else
    ./train 1 ../model2_01.txt ../seq_model_01.txt ../model2_01.txt
    ./train 1 ../model2_02.txt ../seq_model_02.txt ../model2_02.txt
    ./train 1 ../model2_03.txt ../seq_model_03.txt ../model2_03.txt
    ./train 1 ../model2_04.txt ../seq_model_04.txt ../model2_04.txt
    ./train 1 ../model2_05.txt ../seq_model_05.txt ../model2_05.txt
    ./test modellist2.txt ../testing_data1.txt ../result2.txt
    ./a.out ../result2.txt ../score2.txt
  fi
done
