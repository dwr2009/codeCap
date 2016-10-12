"use strict";
(function( ){

		var name = "Service Info";
				
	var ServiceInfos = function(){
		var parent = new window.MainMenuScreen( name );
		var self = Object.create(parent);
		self.$root.attr('id','service_info');
		
		var $service_info_table = $('<div>').addClass('service_info_table').appendTo(self.$root);
			
		var $service_info_lable          = $('<div>').addClass('first_lable').appendTo($service_info_table);		
		var $service_info_plan           = $('<div>').addClass('column_left ').append($TR('service_info_plan :: plan')).appendTo($service_info_lable);
		var $service_info_updatetime     = $('<div>').addClass('column_left ').append($TR('service_info_updatetime :: updatetime')).appendTo($service_info_lable);
		
		var $service_info_second_lable = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_name         = $('<div>').addClass('column_left').append($TR('service_info_name :: name')).appendTo($service_info_second_lable);
		var $service_info_endtime      = $('<div>').addClass('column_left').append($TR('service_info_endtime :: endtime')).appendTo($service_info_second_lable);		

		var $service_info_third_lable = $('<div>').addClass('lable').appendTo($service_info_table);
        var $service_name = $( '<div>' ).addClass( 'column_left' ).appendTo( $service_info_third_lable ) ;
        var $service_endtime = $( '<div>' ).addClass( 'column_left' ).appendTo($service_info_third_lable );											
		
		var $service_info_lable4 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable5 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable6 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable7 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable8 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable9 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable10 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable11 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable12 = $('<div>').addClass('lable').appendTo($service_info_table);
		var $service_info_lable13 = $('<div>').addClass('lable').appendTo($service_info_table);
		
		var $service_info_reminder = $('<div>').addClass('lastlable').append($TR('service_info_reminder :: remindinfo')).appendTo($service_info_table);		
	 			
		var updateScreen = function( ){
			var currentlang = window.Platform.settings.getLanguage();
			console.log("currentlang = "+ currentlang);
			var lang = {
				language:currentlang.toUpperCase(),
			}
			var accountProfile = Platform.ca.GetAccountProfile(lang);
			if(accountProfile.retVal == 0){
				console.log("accountProfile.lastUpdateDate="+accountProfile.lastUpdateDate);
		        var date_string = window.Format.formatDateString(accountProfile.lastUpdateDate) ;
		        $service_info_updatetime.text( TR('service_info_updatetime :: updatetime') + date_string ) ;
		        $service_name.text(accountProfile.servicePlan);
		        $service_endtime.text(accountProfile.serviceEndDate);	        
			}
		} ;
		

	 	self.onKey = function( ) {
					return true ;
		} ;
		
		self.onShow = function( ) {
			updateScreen( ); 
		} ;
		
		self.onKeyUP = function( ) {
			list.selectPrev( ) ;
		} ;

		self.onKeyDOWN = function( ) {
			list.selectNext( ) ;
		} ;		
		return self ;
	} ;
		
			
			window.AppManager.addScene( name,ServiceInfos ) ;
})();