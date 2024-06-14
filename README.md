/* Limits */  
Input file extension must be '.fdf'.  
Vertical input 'z' must be between -30,000 and +30,000.  
Translation/shifting is limited to 50% outside the window width and 100% of window height.  
Zoom is limited to the fewer pixels of the window width and height.  

/* User interaction with the map */  
* ESC / MLX_KEY_ESCAPE: close the window and quit the program in a cleanly.  

* \- / MLX_KEY_KP_SUBTRACT: zoom out.  
* \+ / MLX_KEY_KP_ADD: zoom in.  
Map zoom increment is 2.5%.  

* MLX_KEY_UP: move the image up.  
* MLX_KEY_LEFT: move the image left.  
* MLX_KEY_RIGHT: move the image right.  
* MLX_KEY_DOWN: move the image down.  
Map shifting increment is 10 pixels.  
	
* E / MLX_KEY_E: while looking from the side, reduce viewing angle.  
* D / MLX_KEY_D: while looking from the side, increase viewing angle.  
Map rotation increment is 0.025 radians 1.4323945  

/* Submodules */  
If compilation doesn't work due to MLX42 library problem / To add MLX42 library,  
run in command line:  
`git submodule add --force git@github.com:codam-coding-college/MLX42.git`

/* Colors */  
Assumed color coding:  
`fdf input	=	MiniLibX		=	MLX42`  
`				0xAARRGGBB	=	0xRRGGBBAA`  
`0x12345678	=	0x12345678	=	0x34567812`  
`0x123456	=	0xFF123456	=	0x123456FF`  
`0x1234		=	0x11223344	=	0x22334411`  
`0x12		=	0x12FFFFFF	=	0xFFFFFF12`  
Missing channels are assumed to be have value FF  

/* Leaks */
LeakSanitizer sees a problems with 'mlx_init', but this is happening inside the library code.
"Summary: 7 leaks, 512 B lost."
