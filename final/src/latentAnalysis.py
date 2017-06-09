import sys
import keras
import numpy as np
import theano
import theano.sandbox.cuda.dnn
import pandas as pd
import sklearn 
import nltk

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
count = np.zeros(( doc_num, voc_num))
for i in range( doc_num ):
  row = int( train_data['doc_id'][i] )
  col = int( train_data['word_id'][i] )
  count[row][col] = train_data['num_of_word'][i]  


#model.add(Dense(units = 64, input_dim = 100))
#model.add(Activation('relu'))
#model.add(Dense(units = 10))
#model.add(Activation('softmax'))

#model.compile(loss='categorical_crossentropy',
#              optimizer='sgd',
#              metrics=['accuracy'])
#model.fit(x_train, y_train, epochs = nb_epoch_1, batch_size = batch_size)
#loss = model.evaluate(x_test, y_test, batch_size = batch_size)
