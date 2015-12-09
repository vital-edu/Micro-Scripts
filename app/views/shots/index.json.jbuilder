json.array!(@shots) do |shot|
  json.extract! shot, :id, :shot_date
  json.url shot_url(shot, format: :json)
end
