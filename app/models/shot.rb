class Shot < ActiveRecord::Base
	belongs_to :medicine

	validates :shot_date, presence: true
end
