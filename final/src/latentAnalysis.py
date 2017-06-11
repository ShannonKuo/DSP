import sys
import keras
import numpy as np
import theano
import theano.sandbox.cuda.dnn
import pandas as pd
import sklearn 
import nltk

from math import log
from sklearn.datasets import fetch_20newsgroups
from sklearn.decomposition import TruncatedSVD
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.feature_extraction.text import HashingVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import Normalizer
from sklearn import metrics
from sklearn.cluster import KMeans, MiniBatchKMeans
from sklearn import manifold
from nltk.stem import RegexpStemmer
from nltk.tokenize import RegexpTokenizer
from nltk import stem
from nltk.stem.lancaster import LancasterStemmer
from nltk.stem import WordNetLemmatizer
from nltk.corpus import stopwords
from optparse import OptionParser
  
from keras.layers.normalization import BatchNormalization
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense, Activation, Dropout, Flatten
from keras.optimizers import SGD   #for momentum, learning rate
from keras.utils import np_utils

nb_epoch_1 = 100
batch_size = 32
valid = 50
_train = 1
doc_num = 11269
voc_num = 61188
sep = ' '


op = OptionParser()
op.add_option("--lsa",
              dest="n_components", type="int",
              help="Preprocess documents with latent semantic analysis.")
op.add_option("--no-minibatch",
              action="store_false", dest="minibatch", default=True,
              help="Use ordinary k-means algorithm (in batch mode).")
op.add_option("--no-idf",
              action="store_false", dest="use_idf", default=True,
              help="Disable Inverse Document Frequency feature weighting.")
op.add_option("--use-hashing",
              action="store_true", default=False,
              help="Use a hashing feature vectorizer")
op.add_option("--n-features", type=int, default=10000,
              help="Maximum number of features (dimensions)"
                   " to extract from text.")
op.add_option("--verbose",
              action="store_true", dest="verbose", default=False,
              help="Print progress reports inside k-means algorithm.")
(opts, args) = op.parse_args()

opts.n_components = 20




dtypes1 = { 'doc_id': np.str, 'word_id': np.str, 'num_of_word': np.str }
dtypes2 = { 'label': np.str }
dtypes3 = { 'topic': np.str, 'label': np.str }
train_data = pd.read_csv( "../data/train_data.csv",
                          usecols =  ['doc_id', 'word_id', 'num_of_word'],
                          dtype = dtypes1, sep = sep  )
train_label = pd.read_csv( "../data/train_label.csv", usecols = ['label'], dtype = dtypes2, sep = sep )
train_map = pd.read_csv( "../data/train_map.csv", usecols = ['topic', 'label'], dtype = dtypes3, sep = sep )
test_data = pd.read_csv( "../data/test_data.csv",
                          usecols =  ['doc_id', 'word_id', 'num_of_word'],
                          dtype = dtypes1, sep = sep  )
test_label = pd.read_csv( "../data/test_label.csv", usecols = ['label'], dtype = dtypes2, sep = sep )
test_map = pd.read_csv( "../data/test_map.csv", usecols = ['topic', 'label'], dtype = dtypes3, sep = sep )
#output_log = open("output_log.csv", "w")
dtypes4 = { 'id': np.str, 'value': np.str }
log_table = pd.read_csv( "output_log.csv", usecols = ['id', 'value'], dtype = dtypes4, sep = sep )

tf = np.zeros(( doc_num, voc_num ))
total = np.zeros( doc_num )
word = np.zeros( voc_num )
x_train = np.zeros(( doc_num, voc_num ))
y_train = np.zeros(( doc_num, 20 ))

#for i in range( doc_num ):
  #output_log.write( str(i+1) + " " + str(log(i+1)) + "\n" )
  #output_log.write( " " )
  #output_log.write( log(i+1) )
  #output_log.write("\n")
#print("output finish")
for i in range( len( train_data['doc_id'] ) ):
  if (i % 100 == 0):
    print ( float(i) / len(train_data['doc_id']) * 100)
  row = int( train_data['doc_id'][i] ) - 1
  col = int( train_data['word_id'][i] ) - 1
  tf[row][col] = int( train_data['num_of_word'][i] ) 
  word[col] += 1
  total[row] += int( train_data['num_of_word'][i] )
print("finish 1")

temp = log( doc_num )
for j in range( voc_num ):
  #log_word = log( word[j]+1 );
  log_word = float(log_table["value"][word[j]])
  if ( j % 100 == 0 ):
    print (float(j) / voc_num * 100)
  for i in range( doc_num ):
    tf[i][j] = tf[i][j] / total[i]
    if ( doc_num < word[j] ):
			print ("error")
			print (word[j])
    x_train[i][j] = tf[i][j] * ( temp - log_word )

print("finish 2")

for i in range( doc_num ):
  print ( train_label['label'][i] )
  y_train[i][int(train_label['label'][i])-1] = 1;
print("finish 3")
#for i in range( len( train_data['doc_id'] ) ):

del tf
del total
del word
model = Sequential()
model.add(Dense(units = 64, input_shape = (voc_num, )))
model.add(Activation('relu'))
model.add(Dense(20))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy',
              optimizer='sgd',
              metrics=['accuracy'])
model.fit(x_train, y_train, epochs = nb_epoch_1, batch_size = batch_size)
model.save( "model" )
#model.fit(x_train, y_train, epochs = nb_epoch_1)
#loss = model.evaluate(x_test, y_test, batch_size = batch_size)
