var LabelView = new window.Class(

	// parent
	window.Widget,

	// constructor
	function( description ) {
		Widget.call( this, description.name ) ;

		this.$root.addClass( 'label_view' ) ;
		this.$panel = $( '<div>' ).addClass( 'panel' ).appendTo( this.$root ) ;
		this.$name  = $( '<div>' ).addClass( 'name' ).append( $TR( description.name ) ).appendTo( this.$panel ) ;
		this.$value = $( '<div>' ).addClass( 'value' ).append( $TR( description.value ) ).appendTo( this.$panel ) ;
	},

	// prototype
	{
	        setText: function ( text ) {
			this.$value.text ( text ) ;
	        },
        
	        getText: function ( ) {
			return this.$value.text();
	        },
	}

) ;