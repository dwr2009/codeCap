"use strict";
(function( ){

		var name = "TemAccount";
		

		
	var Temaccount = function(){
			var parent = new window.MainMenuScreen( name );
			var self = Object.create(parent);
			self.$root.attr('id','temaccount');
			
			var $temaccount_array = $('<div>').addClass('temaccount_array').appendTo(self.$root);
			var $temaccount_first = $('<div>').addClass('temaccount_first').appendTo($temaccount_array);
			var $temaccount_second = $('<div>').addClass('temaccount_second').appendTo($temaccount_array);

			var updateScreen = function(){
				$temaccount_first.text(TR("tem :: This function will be available soon"));
				$temaccount_second.text(TR("tem :: Enquiry on service plan"));
			}

			self.onKey = function( ) {
				return true ;
			} ;

			self.onShow = function(){
				updateScreen();
			}


												
		return self ;
	} ;
		
			
			window.AppManager.addScene( name,Temaccount ) ;
})();