
idf_component_register(
	SRCS
	"main.c"
	"../components/mlx90614/mlx90614_Handler.c"        
	"../components/SDCard/SDCardHandler.c"

	
	INCLUDE_DIRS
	"."
	"../components/mlx90614"
	"../components/SDCard"		    
  )