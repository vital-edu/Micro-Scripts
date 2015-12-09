class CreateMedicines < ActiveRecord::Migration
  def change
    create_table :medicines do |t|
      t.string :name
      t.text :description
      t.integer :amount
      t.integer :shot
      t.integer :max_shot

      t.timestamps null: false
    end
  end
end
