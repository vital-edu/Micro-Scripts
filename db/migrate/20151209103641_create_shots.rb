class CreateShots < ActiveRecord::Migration
  def change
    create_table :shots do |t|
      t.time :shot_date
      t.belongs_to :medicine

      t.timestamps null: false
    end
  end
end