"use strict";
(function( ) {

	var prefix = 'templates/dev/' ;

	var loading_job = window.Loader.loadStyles( [
		prefix + 'css/style.css',
	] ) ;


	var Template = new function( ) {

		this.prefix = prefix ;

		this.init = function( callback ) {
			loading_job.onDone( callback ) ;

			$( '<div>' ).text( 'Template is DEV' ).css( {
				'position': 'fixed',
				'bottom': '15px',
				'right': '15px',
			 } ).appendTo( 'body' ) ;
		} ;

	} ;

	window.Template = Template ;

})( ) ;
