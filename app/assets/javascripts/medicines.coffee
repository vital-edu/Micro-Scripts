# Place all the behaviors and hooks related to the matching controller here.
# All this logic will automatically be available in application.js.
# You can use CoffeeScript in this file: http://coffeescript.org/

updateAmount = ->
  id = $('#medicine_id').val()
  
  $.get '/get_amount/'+id, (response) ->
    $('#amount').html response.amount

    window.setTimeout ->
      updateAmount()
    , 500


updateAmount()
