### Goal of the project  
Creating a simple wireframe model representation of a 3D landscape by linking various points (x, y, z) thanks to line segments (edges).  

### Restrictions   
Input file extension must be `.fdf`.  
Vertical input `z` must be between -30,000 and +30,000.  
Translation/shifting is limited to 50% outside the window width and 100% of window height.  
Zoom is limited to the fewer pixels of the window width and height.  

### User interaction with the map   
`ESC`: close the window and quit the program in a clean way.  

Numpad `-`: zoom out.  
Numpad `+`: zoom in.  
Map zoom increment is 2.5%.  

`MLX_KEY_UP`: move the map up.  
`MLX_KEY_LEFT`: move the map left.  
`MLX_KEY_RIGHT`: move the map right.  
`MLX_KEY_DOWN`: move the map down.  
Map shifting increment is 10 pixels.  

`E`: while looking at the map from above, reduce viewing angle.  
`D`: while looking at the map from above, increase viewing angle.  
Map rotation increment is 0.025 radians ≈ 1.4°  

### Submodules   
If compilation does not work due to MLX42 library problem / To add MLX42 library,  
run in command line:  
`git submodule add --force git@github.com:codam-coding-college/MLX42.git`

### Colors   
Colors of provided test maps were converted based on the assumption that they use the [MiniLibX scheme](https://harm-smits.github.io/42docs/libs/minilibx/colors.html), while [MLX42 scheme](https://github.com/codam-coding-college/MLX42/blob/master/docs/Colors.md) is being used here:  
`fdf input	=	MiniLibX		=	MLX42`  
`				0xAARRGGBB	=	0xRRGGBBAA`  
`0x12345678	=	0x12345678	=	0x34567812`  
`0x123456	=	0xFF123456	=	0x123456FF`  
`0x1234		=	0x11223344	=	0x22334411`  
`0x12		=	0x12FFFFFF	=	0xFFFFFF12`  
Missing channels are assumed to be have value FF.  

### Leaks   
LeakSanitizer sees a problems with `mlx_init`, but this is happening inside the library code.  
"Summary: 7 leaks, 512 B lost."
