"use strict";
(function( ) {

	var Class = function( parent, init, proto ) {

		var constructor = function( ) {
			init.apply( this, arguments ) ;
		} ;

		constructor.prototype = new function( ) {
			var self = Object.create( parent.prototype ) ;

			_.extend( self, proto ) ;

			return self ;
		} ;

		return constructor ;
	} ;

	window.Class = Class ;

})( ) ;
