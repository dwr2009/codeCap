"use strict";
(function( ) {

	var makeScriptNode = function( url, callback ) {
		var node = document.createElement( 'script' ) ;
		node.onload = callback ;
		node.src = url ;
		return node ;
	} ;

	var makeStyleNode = function( url, callback ) {
		var node = document.createElement( 'link' ) ;

		// node.onload = callback ; // link.onload doesn't work on older browsers
		node.rel = 'stylesheet' ;
		node.type = 'text/css' ;
		node.href = url ;

		callback && callback( ) ; // call the callback manually

		return node ;
	} ;


	var DOMLoader = function( type, files ) {
		var current = -1 ;
		var len = files.length ;
		var finished = false ;
		var callback = null ;

		var loadNext = function( ) {
			//console.log( "load next" )
			current++ ;
			//console.log( "load next file = " + files[current] );
			if( current == len ) {
				finished = true ;
				callback && callback( ) ;
				return ;
			}
			if (files[current] === "undefined" ) {
				console.log( "load next file is undefined - skip" );
			} else {

				var url = files[current] ;
				var node = null ;
				//console.log( "load next url = " + url )

				switch( type ) {
					case 'script': node = makeScriptNode( url, loadNext ) ; break ;
					case 'style':  node = makeStyleNode(  url, loadNext ) ; break ;
				}

				document.head.appendChild( node ) ;
			};
		} ;

		loadNext( ) ;


		this.onDone = function( done ) {
			if( finished ) {
				done && done( ) ;
				return ;
			}

			callback = done ;
		} ;
	} ;


	var Loader = new function( ) {

		this.loadScripts = function( scripts ) {
			return new DOMLoader( 'script', scripts )
		} ;

		this.loadStyles = function( styles ) {
			return new DOMLoader( 'style', styles )
		} ;

		this.loadJson = function( url ) {
			return new JsonLoader( url ) ;
		} ;

	} ;

	window.Loader = Loader ;

})( ) ;
