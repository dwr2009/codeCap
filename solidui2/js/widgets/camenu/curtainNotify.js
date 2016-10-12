var curtainNotify = new window.Class(

	// parent
	window.Widget,

	// constructor
	function( ) {
		Widget.call( this,"curtain Notify ") ;

		this.$root.addClass( 'curtainNotify' ) ;
	},

	// prototype
	{
		ShowCurtainNotify:function(param){				
				if (param.curtainCode != 0) {
						this.$root.css('visibility','visible');
				}
				else {
						this.$root.css('visibility','hidden');
				}				
		}	
	}

) ;