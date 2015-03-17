# 眾生平等組合投資工具
# 嘗試用literate coffeescript來寫

## 設置組合品種
# 將由用戶提出請求,根據投資品種屬性,系統採納或拒絕這一品種.

# 設計錯誤,只需要當前組合和可能組合即可,待修改
# 當前投資品種
Vehicles = new Mongo.Collection 'Vehicles'
# 可配對品種記錄
Pairs = new Mongo.Collection 'Pairs'
# 當前配對情形
CurrentPairs = new Mongo.Collection 'CurrentPairs'

getPrice = (code) ->
  code

getName = (code) ->
  code

CurrentPairs.helpers
  組合數: ->
    @組合.length

Vehicles.helpers
  amount: ->
    return 0 if @volumn is 0
    # 查詢品種當前價格

Meteor.methods
  addPair: (obj) ->
    # 先須檢查該品種是否符合要求
    # obj.candidateCode obj.targetCode

  addCurrentPair: (pair) ->
    CurrentPairs.insert pair

  removePair: (名) ->
    CurrentPairs.remove 名: 名

  addVehicle: (code) ->
    Vehicles.insert 代碼: code

  removeVehicle: (code) ->
    Vehicles.remove 代碼: code



if Meteor.isClient
  Meteor.subscribe 'vehiclesChannel'
  Meteor.subscribe 'pairsChannel'
  Meteor.subscribe 'currentPairsChannel'

  Template.ekuaTable.helpers
    vehicles: -> Vehicles.find {}
    fields: ['代碼', '數量', '資產', '配額', '操作']

  Template.ekuaTable.events
    'dblclick .reactive-table tr': (e) ->
        e.preventDefault();
        Meteor.call 'removeVehicle', @代碼

    'keypress #newVehicle': (e, t)->
      # 如何用jQuery加一段提示進來呢
      if e.which is 13
        code = (t.find '#newVehicle').value
        Meteor.call 'addVehicle', code
        ($ "<p> #{(t.find '#newVehicle').value }</p>").appendTo $ '.container'


if Meteor.isServer

  Meteor.publish 'vehiclesChannel', (userId) ->
    Vehicles.find {}
  Meteor.publish 'pairsChannel', (userId) ->
    Pairs.find {}
  Meteor.publish 'currentPairsChannel', (userId) ->
    CurrentPairs.find {}

  Meteor.startup ->
    pair = {}
    Meteor.call 'addCurrentPair', pair unless CurrentPairs.findOne()?
