"use strict"; (function() {

    var progressStrip = new window.Class(
    	// parent
    	window.Widget,
    
    	// constructor
    	function() {
    		Widget.call( this,"progress Strip") ;
    		
    		this.$root.addClass( 'ProgressStrip' ) ;
    		this.$panel   = $( '<div>' ).appendTo( this.$root ) ;
		this.$strips_bg  = $( '<img src = ./templates/hkc/images/progress_bg.png>' ).addClass( 'strips_bg' ).appendTo( this.$panel ) ;
    		this.$strips  = $( '<img src = ./templates/hkc/images/progress.png>' ).addClass( 'strips' ).appendTo( this.$panel ) ;  	
    	},
  
    	// prototype
    	{
    		ShowProgressStrip:function(param){
    				console.log("param.progress = "+ param.progress);
    				if(param.progress > 100){
    						this.$root.css('visibility','hidden');
    					}
    				else{
    						this.$root.css('visibility','visible');
    						this.$strips.css('width', (this.$strips_bg.width()-20)*param.progress/100);
    				}
    		},
    	}
	);
	
	window.progressStrip = progressStrip ;

})( ) ;