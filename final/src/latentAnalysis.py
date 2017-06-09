import sys
import keras
import numpy as np
import theano
import theano.sandbox.cuda.dnn

from keras.layers.normalization import BatchNormalization
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense, Activation, Dropout, Flatten
from keras.optimizers import SGD   #for momentum, learning rate
from keras.utils import np_utils

nb_epoch_1 = 100
batch_size = 32
valid = 50

directory_path = sys.argv[1]

if (_train == 1)
  train_data = open( "../data/train.data", "r" )
	train_label = open( "../data/train.label", "r" )
	train_map = open( "../data/train.map", "r" )
else 
  train_data = open( "../data/test.data", "r" )
	train_label = open( "../data/test.label", "r" )
	train_map = open( "../data/test.map", "r" )


model.add(Dense(units = 64, input_dim = 100))
model.add(Activation('relu'))
model.add(Dense(units = 10))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='sgd',
		          metrics=['accuracy'])
model.fit(x_train, y_train, epochs = nb_epoch_1, batch_size = batch_size)
loss = model.evaluate(x_test, y_test, batch_size = batch_size)
