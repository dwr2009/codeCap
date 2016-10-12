"use strict";
(function( ) {

	var salt = 0 ;
	var max_salt = 9999999 ;

	var getId = function( ) {
		var s = salt++ ;
		if( salt > max_salt ) {
			salt = 0 ;
		}
		return Date.now( ) + s ;
	}


	var text_nodes = {} ;

	var registerNode = function( node ) {
		var $node = $( node ) ;
		var id = $node.data( 'id' ) ;
		text_nodes[id] = $node ;
		updateText( $node ) ;
	} ;

	var unregisterNode = function( node ) {
		var $node = $( node ) ;
		var id = $node.data( 'id' ) ;
		delete text_nodes[id] ;
	} ;

	var notifyNodes = function( ) {
		for( var i in text_nodes ) {
			var $node = text_nodes[i] ;
			updateText( $node ) ;
		}
	} ;

	var updateText = function( $node ) {
		var string = $node.data( 'string' ) ;
		var text = window.TR( string ) ;
		$node.html( text ) ;
	} ;

	// var observer = new MutationObserver( function( mutations ) {
	// 	mutations.forEach( function( mutation ) {
	// 		if( mutation.type != "childList" ) { return ; }

	// 		var forAllSmartTextNodes = function( collection, callback ) {
	// 			for( var i = 0 ; i < collection.length ; i++ ) {
	// 				var root_node = collection[i] ;
	// 				var $root_node = $( root_node ) ;

	// 				var $nodes = $root_node.find( '.smart_text' ) ;
	// 				if( $root_node.hasClass( 'smart_text' ) ) {
	// 					$nodes = $nodes.add( root_node ) ;
	// 				}
	// 				_.each( $nodes, callback ) ;
	// 			}
	// 		} ;

	// 		forAllSmartTextNodes( mutation.addedNodes, registerNode ) ;
	// 		forAllSmartTextNodes( mutation.removedNodes, unregisterNode ) ;

	// 	} ) ;
	// } ) ;

	// observer.observe( document.body, { childList: true, subtree: true } ) ;


	var loadLanguage = function( language ) {
                $( 'html' ).attr( 'lang', language );
		return window.Loader.loadScripts( [
			'js/translations/' + language + '.js',
		] ) ;
	} ;


	var language = window.Platform.settings.getLanguage();
	console.log( "language is", language ) ;

	var loading_job = loadLanguage( language ) ;


	window.Translation = new function( ) {

		this.init = function( done ) {
			this.language = language ;

			loading_job.onDone( done ) ;
		} ;

		this.translate = function( string ) {
			var translation = window.STRINGS[string] ;

			if( !translation ) {
				// console.warn( "TR: string '" + string + "' has no translation." ) ;
				translation = string ;
			}

			return translation ;
		} ;

		this.createTextNode = function( string ) {
			var $text = $( '<span>' )
				.addClass( 'smart_text' )
				.data( {
					id: getId( ),
					string: string,
				} ) ;

			registerNode( $text ) ; // remove if MutationObserver exists

			return $text ;
		} ;

		this.setLanguage = function( language ) {

			var callback = function( ) {
				this.language = language ;
				window.settings.language = language ;
				window.Platform.settings.setLanguage(language);
				notifyNodes( ) ;
			} ;

			var loading_job = loadLanguage( language ) ;
			loading_job.onDone( _.bind( callback, this ) ) ;
		} ;

		this.updateNode = function( $node, string ) {
			$($node).data( 'string', string );
			var text = window.TR( string ) ;
			$($node).html( text ) ;		
		} ;
	} ;

	window.TR        = _.bind( window.Translation.translate,      window.Translation ) ;
	window.$TR       = _.bind( window.Translation.createTextNode, window.Translation ) ;
	window.TR_UPDATE = _.bind( window.Translation.updateNode,     window.Translation ) ;
	
})( ) ;
