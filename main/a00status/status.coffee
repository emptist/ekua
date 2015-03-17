@Status = new Mongo.Collection 'Status'
Status.helpers
  品種數: -> (CurrentPairs.find {}).length
  配額: -> @當前餘額/8

Meteor.methods
  setStatus: (id, status) ->
    Status.update id, status


if Meteor.isClient
  Meteor.subscribe 'statusObj'

  Template.status.helpers
    status: -> Status.find {}
    fields: ['當前餘額','品種數','配額']
    當前餘額: -> if (s=Status.findOne())? then s.當前餘額 else 0.0

  Template.status.events
    'keypress #update': (e, t) ->
      if e.which is 13
        Meteor.call 'setStatus',
          Status.findOne()._id, 當前餘額: (t.find '#update').value

  Template.status.helpers
    doc: -> Status.findOne()


if Meteor.isServer
  Meteor.startup ->
    if (Status.find {}).fetch().length <1
      Status.insert 當前餘額: 10
      console.log "done"

  Meteor.publish 'statusObj', (userId) ->
    Status.find {}
