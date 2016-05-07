from sklearn import neighbors,naive_bayes,cluster,linear_model,ensemble,svm
from sklearn.datasets import load_svmlight_file
import pandas as pd
import sys
import numpy as np
import scipy as sp

if(sys.argv[1]=='knn'):
	train = pd.read_table('trainexpknn.txt',sep=" ",header=None)
	test = pd.read_table('testexpknn.txt',sep=" ",header=None)
	features_train  =  train.iloc[:,0:140]    # X includes columns 0,1,2,3
	features_train_labels  =  train[140]   # Get last column
	features_test  =  test.iloc[:,0:140]    # X includes columns 0,1,2,3
	features_test_labels  =  test[140]
else:

	features_train,features_train_labels  =  load_svmlight_file('trainexpsvmblow.txt')  
	features_test,features_test_labels  =  load_svmlight_file('testexpsvmblow.txt')    # X includes columns 0,1,2,3
#print features_train.to_array()
#print features_train_labels.toarray()
'''
knn_clf = neighbors.KNeighborsClassifier(n_neighbors=1,weights="uniform",n_jobs=-1)
knn_clf.fit(features_train, features_train_labels)
accuracy=knn_clf.score(features_train,features_train_labels)
print ("KNN train: "+ str(accuracy))
accuracy=knn_clf.score(features_test,features_test_labels)
print ("KNN test: "+ str(accuracy))

NB_clf = naive_bayes.GaussianNB()
NB_clf.fit(features_train, features_train_labels)
accuracy=NB_clf.score(features_train,features_train_labels)
print ("Naive Bayes train: "+ str(accuracy))
accuracy=NB_clf.score(features_test,features_test_labels)
print ("Naive Bayes test: "+ str(accuracy))
'''
'''
KM_clf = cluster.KMeans(n_clusters=111,precompute_distances=True,n_jobs=-1)
KM_clf.fit(features_train, features_train_labels)
accuracy=knn_clf.score(features_test,features_test_labels)
print ("KMeans: "+ str(accuracy))
'''
'''
#for r in [0.1,0.001,0.2,0.0001,0.3,1.0,0.4,0.6,0.05,0.5]:
Perceptron_clf = linear_model.Perceptron(eta0=0.01,n_iter=400,n_jobs=-1)
Perceptron_clf.fit(features_train, features_train_labels)
accuracy=Perceptron_clf.score(features_train,features_train_labels)
print ("Perceptrontrain: "+ str(accuracy))
accuracy=Perceptron_clf.score(features_test,features_test_labels)
print ("Perceptron test: "+ str(accuracy))
'''
'''
AB_clf = ensemble.AdaBoostClassifier()
AB_clf.fit(features_train, features_train_labels)
accuracy=AB_clf.score(features_test,features_test_labels)
print ("AdaBoost: "+ str(accuracy))
'''
#'''
for i in [0.0001,0.001,0.01,0.1,1,10,20,30,40,50,60,70,80,90,100]:#xrange(21,40):
	SVM_clf = svm.SVC(C=i)#,kernel='poly',degree=2)
	SVM_clf.fit(features_train, features_train_labels)
	accuracy=SVM_clf.score(features_train,features_train_labels)
	print ("SVM train: "+ str(accuracy))
	accuracy=SVM_clf.score(features_test,features_test_labels)
	print ("SVM test: "+ str(accuracy))
#'''

