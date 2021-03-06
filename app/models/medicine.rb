class Medicine < ActiveRecord::Base
	has_many :shots

	# Validations
	validates :name, presence: true, uniqueness: true, length: { in: 4..20 }
	validates :description, presence: true, length: { in: 4..200 }
	validates :amount, presence: true, numericality: { greater_than_or_equal_to: 0 }
	validates :shot, presence: true, numericality: { greater_than_or_equal_to: 0 }
	validates :max_shot, presence: true, numericality: { greater_than_or_equal_to: 0 }

def next_shot
    now = Time.now.hour

    if self.shots.nil?
      return -1
    end

    a = self.shots
    a.sort
    a.each do |p|
      puts "NOW: #{now}"
      puts "SHOT_DATE: #{p.shot_date.hour}"
      if p.shot_date.hour > now
        return "#{p.shot_date.hour - now}h"
      end
    end

    return '-'
  end

private
  def format_date time
    time.strftime("%H:%M:%S")
  end
end
