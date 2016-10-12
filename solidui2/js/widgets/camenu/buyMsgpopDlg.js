var buyMsgpopDlg = new window.Class(

	// parent
	window.Widget,

	// constructor
	function() {
		Widget.call( this,"buyMsg popDlg ") ;

		this.$root.addClass( 'buyMsgpopDlg' ) ;
		$buyMsgpopMsg = $( '<div>' ).addClass( 'buyMsgpopmsg' ).appendTo( this.$root ) ;
		this.$buyMsgpopMsg = $buyMsgpopMsg;
	},

	// prototype
	{        
        ShowBuyMessage:function(Msg) {
        									
 				this.$buyMsgpopMsg.html(window.$TR(Msg));
        },         
	}

) ;