var popDlg = new window.Class(

	// parent
	window.Widget,

	// constructor
	function() {
		Widget.call( this,"pop Dlg ") ;

		this.$root.addClass( 'popDlg' ) ;
		$popMsg = $( '<div>' ).addClass( 'popmsg' ).appendTo( this.$root ) ;
		this.$popMsg = $popMsg;
	},

	// prototype
	{
				
        RequestFeeding:function(str) {			
      			console.log("RequestFeeding is function");			
            this.$popMsg.text(TR(str));
        },              
        showActionMessage:function(str) {
            this.$popMsg.text(TR(str));
        },
        showDlgMsg:function(str) {
        		console.log("buy successful");
            this.$popMsg.text(TR(str));
        },
        showSmcStatus:function(){
        	console.log("NSTV CA screen :: Please inserted Smartcard");
        	this.$popMsg.text('NSTV CA screen :: Please inserted Smartcard');	
        },
	}

) ;