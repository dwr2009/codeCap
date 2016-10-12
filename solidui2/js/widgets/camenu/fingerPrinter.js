var fingerPrinter = new window.Class(

	// parent
	window.Widget,

	// constructor
	function() {
		Widget.call( this,"finger Printer ") ;

		this.$root.addClass( 'fingerPrinter' ) ;
		this.$Message = $('<div>').addClass( 'fingermessage' ).appendTo( this.$root ) ;
	},

	// prototype
	{
		ShowFingerMessageExt:function(param){ 			  
				console.log("ShowFingerMessageExt is function!!!");
				if (param.fingerMessage == 0) {
						this.$root.css('visibility','hidden');
				}
				else {
						var monitorWidth = $('#interface').width();
						var monitorHeight = $('#interface').height();
						var fingerWidth = $('.fingerPrinter').width();
						var fingerHeight = $('.fingerPrinter').height();
					  var xValue = Math.floor(Math.random()*(monitorWidth - fingerWidth)) + "px";
						this.$root.css('left', xValue);
						var yValue = Math.floor(Math.random()*(monitorHeight - fingerHeight)) + "px";
						this.$root.css('top' , yValue);
						this.$root.css('visibility','visible');
						this.$Message.text(param.fingerMessage); 
				}
				
		}	
	}

) ;