import sys
import keras
import numpy as np
import theano
import theano.sandbox.cuda.dnn
import pandas as pd
import nltk
import os
import string
os.environ["THEANO_FLAGS"] = "mode=FAST,device=gpu,floatX=float32"
from math import log
  
from keras.layers.normalization import BatchNormalization
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense, Activation, Dropout, Flatten
from keras.optimizers import SGD   #for momentum, learning rate
from keras.utils import np_utils

_train = 0
if sys.argv[1] == "train":
  _train = 1
  print("training...")
else:
  _train = 0
  print("testing...")

nb_epoch_1 = int(sys.argv[2])
batch_size = 32
valid = 50
sep = ' '
_loadFeature = 0

dtypes1 = { 'doc_id': np.str, 'word_id': np.str, 'num_of_word': np.str }
dtypes2 = { 'label': np.str }
dtypes3 = { 'topic': np.str, 'label': np.str }
print("open file")
if _train == 1:
  t_data = pd.read_csv( "../data/train_data.csv",
                            usecols =  ['doc_id', 'word_id', 'num_of_word'],
                            dtype = dtypes1, sep = sep  )
  t_label = pd.read_csv( "../data/train_label.csv", usecols = ['label'], dtype = dtypes2, sep = sep )
else:
  t_data = pd.read_csv( "../data/test_data.csv",
                            usecols =  ['doc_id', 'word_id', 'num_of_word'],
                            dtype = dtypes1, sep = sep  )
  t_label = pd.read_csv( "../data/test_label.csv", usecols = ['label'], dtype = dtypes2, sep = sep )
dtypes4 = { 'id': np.str, 'value': np.str }
log_table = pd.read_csv( "output_log.csv", usecols = ['id', 'value'], dtype = dtypes4, sep = sep )
doc_num = len( t_label['label'] )
voc_num = 61188

if _loadFeature == 0:
  print( "create feature" )
  x_t = np.zeros(( doc_num, voc_num ))
  total = np.zeros( doc_num )
  word = np.zeros( voc_num )
  for i in range( len( t_data['doc_id'] ) ):
    if (i % 10000 == 0):
      print ( float(i) / len(t_data['doc_id']) * 100),
    row = int( t_data['doc_id'][i] ) - 1
    col = int( t_data['word_id'][i] ) - 1
    x_t[row][col] = int( t_data['num_of_word'][i] ) 
    word[col] += 1
    total[row] += int( t_data['num_of_word'][i] )
  if _train == 1:
    output_feature = open("../data/output_feature_train.csv", "w")
  else:
    output_feature = open("../data/output_feature_test.csv", "w")
  temp = log( doc_num )
  for j in range( voc_num ):
    log_word = temp - float(log_table["value"][word[j]])
    if ( j % 1000 == 0 ):
      print (float(j) / voc_num * 100)
    for i in range( doc_num ):
      x_t[i][j] = x_t[i][j] / total[i] * log_word

  for i in range( doc_num ):
    for j in range( voc_num ):
      output_feature.write(str(x_t[i][j]) + " ")
    output_feature.write('\n') 
  del total
  del word
  del output_feature
  del log_table
 
else:
  print( "load feature" )
  if _train == 1:
    file = open("../data/output_feature_train.csv", "r")
  else:
    file = open("../data/output_feature_test.csv", "r")
  x_t = []
  cnt = 0
  for line in file.xreadlines():
    temp = string.split(line, ' ')
    temp2 = np.zeros(len(temp)-1)
    for i in range( len(temp2)):
      temp2[i] = float(temp[i])
    x_t.append(temp2)
    if cnt % 1000 == 0:
      print( float(cnt) / doc_num * 100 )
    cnt += 1

if _train == 1:
  y_t = np.zeros(( doc_num, 20 ))
  for i in range( doc_num ):
    y_t[i][int(t_label['label'][i])-1] = 1;
  print("start training")
 
  model = Sequential()
  model.add(Dense(units = 64, input_shape = (voc_num, )))
  model.add(Activation('relu'))
  model.add(Dense(20))
  model.add(Activation('softmax'))
  
  model.compile(loss='categorical_crossentropy',
                optimizer='sgd',
                metrics=['accuracy'])
  model.fit(np.array(x_t)[valid:doc_num-1,:], np.array(y_t)[valid:doc_num-1,:], epochs = nb_epoch_1, batch_size = batch_size)
  model.save( "../data/model" + sys.argv[2] + ".h5" )
  loss = model.evaluate(np.array(x_t)[0:valid-1,:], np.array(y_t)[0:valid-1,:], batch_size = batch_size)
else:
  print( "start testing" )
  model = load_model("../data/model" + sys.argv[2] + ".h5")
  output_ans = open("../data/output_ans" + sys.argv[2] + ".txt", "w")
  p = model.predict(np.array(x_t), batch_size = batch_size)
  ans = np.zeros( len(t_label['label']) )
  correct = 0
  for i in range(len(p)):
    maxans = 0
    for j in range (20):
      if p[i][j] > maxans:
        ans[i] = int(j + 1)
        maxans = p[i][j]
    output_ans.write( str(ans[i]) + '\n')
    if int(ans[i]) == int(t_label['label'][i]):
      correct += 1
  print ("correct:" + str(correct))
  print ("final:" + str(float(correct) / doc_num))
  output_ans.write( str(float(correct) / doc_num) )
