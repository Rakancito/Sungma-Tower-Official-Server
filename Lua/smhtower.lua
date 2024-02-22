quest smhtower begin
	state start begin
		when 4019.chat."Entrar a la Torre Infernal Sungma Hee" begin
			say_title(mob_name(4019))
			say("¿Deseas abrir el panel de entrada?")
			local answer = select("Si", "No")
			if answer == 2 then 
				return 
			end
			SungmaHeeTower.Open()
		end
	end
end