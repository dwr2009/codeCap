var showOSDMsg = new window.Class(

	// parent
	window.Widget,

	// constructor
	function() {
		Widget.call( this," OSD Message") ;

		this.$topOSD = $( '<div >' ).addClass( 'topOSD' ).appendTo( this.$root ) ;
		$topOSDMsg = $( '<div>' ).addClass( 'message' ).appendTo( this.$topOSD ) ;
		
		this.$fullOSD = $( '<div>' ).addClass( 'fullOSD' ).appendTo( this.$root ) ;
		$fullOSDMsg = $( '<div>' ).addClass( 'osdmessage' ).appendTo( this.$fullOSD ) ;
		
		this.$halfOSD = $( '<div>' ).addClass( 'halfOSD' ).appendTo( this.$root ) ;
		$halfOSDMsg = $( '<div>' ).addClass( 'osdmessage' ).appendTo( this.$halfOSD ) ;
		
		this.$bottomOSD = $( '<div >' ).addClass( 'bottomOSD' ).appendTo( this.$root ) ;
		$bottomOSDMsg = $( '<div>' ).addClass( 'message' ).appendTo( this.$bottomOSD ) ;
		
		this.$topOSDMsg = $topOSDMsg;
		this.$fullOSDMsg = $fullOSDMsg;
		this.$halfOSDMsg = $halfOSDMsg;
		this.$bottomOSDMsg = $bottomOSDMsg;
	},

	// prototype
	{
		ShowOSDMessage:function(param){
		console.log("ShowOSDMessage is function param.style = "+param.style);
				switch (param.style){
						case 1:
								this.$topOSDMsg.html("<marquee direction=left scrollAmount=1 scrollDelay=1>" + param.message + "</marquee>");		
								$('.topOSD').css('visibility','visible');																
								//this.$topOSDMsg.text(param.message);							
								break;
						case 2:
								this.$bottomOSDMsg.html("<marquee direction=left scrollAmount=1 scrollDelay=1>" + param.message + "</marquee>");
								$('.bottomOSD').css('visibility','visible');
								//this.$bottomOSDMsg.text(param.message);								
								break;
						case 3:
								$('.fullOSD').css('visibility','visible');
								this.$fullOSDMsg.text(param.message);
								break;
						case 4:
								$('.halfOSD').css('visibility','visible');
								this.$halfOSDMsg.text(param.message);
								break;
				}
		},
	  HideOSDMessage:function(param) {
	  			console.log("HideOSDMessage is function param.style = "+param.style);
				switch (param.style){
						case 1:								
								$('.topOSD').css('visibility','hidden');
								break;
						case 2:						
								$('.bottomOSD').css('visibility','hidden');
								break;
						case 3:
								$('.fullOSD').css('visibility','hidden');
								break;
						case 4:
								$('.halfOSD').css('visibility','hidden');
								break;
				}
		}
	}

) ;



