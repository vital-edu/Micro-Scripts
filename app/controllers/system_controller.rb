class SystemController < ApplicationController
	def index
		@medicines = Medicine.all
	end
end
