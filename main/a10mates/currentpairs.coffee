# 眾生平等組合投資工具
@CurrentPairs = new Mongo.Collection 'CurrentPairs'

CurrentPairs.helpers
  組合數: ->
    (CurrentPairs.find {}).fetch().length

Meteor.methods
  addCurrentPair: (add, anti, simi, hrisk) -> # 反向,同向,兩個組合品種,可能沒有同向
    if (t = CurrentPairs.find 代碼: add)?
      CurrentPairs.update t._id,
        反向: anti
        同向: simi
        高風險: hrisk #高風險是指同向品種相對風險
      console.log 'updating'
    else
      CurrentPairs.insert
        代碼: add
        反向: anti
        同向: simi
        高風險: hrisk
      console.log 'inserting'

    if (s = CurrentPairs.find 代碼: simi)?
      CurrentPairs.update s._id,
        同向: add
        高風險: not hrisk
    else
      CurrentPairs.insert
        代碼: simi
        反向: null
        同向: add
        高風險: not hrisk

    if (a = CurrentPairs.find 代碼: anti)?
      CurrentPairs.update a._id, 反向: add
    else
      CurrentPairs.insert
        代碼: anti
        反向: add
        同向: null


  removeFromCurrentPair: (code) ->
    if (o = CurrentPairs.find 代碼:code)?
      if anti = o.反向?
        CurrentPairs.update 代碼: anti, 反向: null
      if simi = a.同向?
        CurrentPairs.update 代碼: simi, 同向: null
      CurrentPairs.remove 代碼: code


if Meteor.isClient
  Meteor.subscribe 'currentPairsChannel'
  Template.currentPairs.helpers
    cp: -> CurrentPairs.find {}
    fields: ['代碼','反向','同向','高風險']

  Template.addVehicle.events
    'click .submit': (e, t) ->
      e.preventDefault()
      Meteor.call 'addCurrentPair',
        t.find('#add').value,
        t.find('#anti').value,
        t.find('#simi').value,
        t.find('#risk').checked

if Meteor.isServer
  Meteor.publish 'currentPairsChannel', (userId) ->
    CurrentPairs.find {}

  Meteor.startup ->
    unless CurrentPairs.findOne()? # 不工作?
      Meteor.call 'addCurrentPair', 150001, 159915, 159902, true
