# 眾生平等組合投資工具
# 嘗試用literate coffeescript來寫

## 設置組合品種
#將由用戶提出請求,根據投資品種屬性,系統採納或拒絕這一品種.

Vehicles = new Mongo.Collection 'Vehicles'

if Meteor.isClient
  Meteor.subscribe 'vehiclesChanel'

  Template.ekuaTable.helpers
    vehicles: -> Vehicles.find {}
    fields: []

  Template.ekuaTable.events
    'keyup #newVehicle': (e, t)->
      # 如何用jQuery加一段提示進來呢
      ($ "<p> #{(t.find '#newVehicle').value }</p>").appendTo $ '#newVehicle'


if Meteor.isServer

  Meteor.publish 'vehiclesChanel', (userId) ->
    Vehicles.find {}
