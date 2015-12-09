json.array!(@medicines) do |medicine|
  json.extract! medicine, :id, :name, :description, :amount, :shot, :max_shot
  json.url medicine_url(medicine, format: :json)
end
