
# coding: utf-8

# # Data loading, storage, and file formats


# ## I added this: Mongodb testing
# http://stackoverflow.com/questions/23300295/cant-connect-to-meteor-with-pymongo
# 首先安裝pymongo:
#   conda install -c https://conda.binstar.org/travis pymongo

# In[62]:

import pymongo
from pandas import *

# 用python來改寫meteor數據庫,並實時反應在應用中
# testing with my meteor project ekua

# In[19]:

conn = pymongo.MongoClient(r'mongodb://localhost:3001/meteor')
conn


# In[35]:

dbns = conn.database_names()
dbns


# In[59]:

conn.meteor.collection_names()


# In[60]:

sysidx = conn.meteor.system.indexes.find({})


# In[61]:

#from pandas import *
DataFrame(list(sysidx))


# In[73]:

stocks = conn.meteor.Vehicles.find({})
dfsts = DataFrame(list(stocks))
dfsts


# In[74]:

dfsts.數量 = [200,100]
#dfsts


# In[81]:

df2 = DataFrame({'代碼':['150153','150154'],
                '數量':[200,100],
                '配額':[0.5,0.5],
                '操作':['買入','賣出']})
df2


# In[118]:

ndf = dfsts.merge(df2)
ndf


# In[126]:

ndf.T.to_dict()


# In[128]:

ndict = ndf.set_index('_id').T.to_dict()
ndict


# In[130]:

ndict.keys()


# In[131]:

for i in ndict:
    print(ndict[i])


# In[ ]:

stocks = conn.meteor.Vehicles
for idx in ndict:
    stocks.update({'_id':idx}, ndict[idx])


# In[136]:

DataFrame(list(stocks.find()))
