"use strict"; 
(function() {

    var name = "Reminder Pop";

    var ReminderPopDlg= function() {

        var parent = new window.Scene(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'reminderPop');
     
        var $headline_text = $('<div>').addClass('headline_text').append($TR('reminderPopDig :: headline text')).appendTo(self.$root);

        var $reminder_channel_name = $('<div>').addClass('reminder_message').appendTo(self.$root);
        var $reminder_event_name = $('<div>').addClass('reminder_message').appendTo(self.$root);
        var $reminder_event_dateTime = $('<div>').addClass('reminder_message').appendTo(self.$root);

	var $reminder_btn = $('<div>').addClass('btnlable').appendTo(self.$root);
        var $confirm_btn_area = $('<div>').addClass('confirmbtn').appendTo($reminder_btn);
        var $reminder_confirm_panel = $('<div>').addClass('panel').append($TR('pincode_btn_confirm :: confirm')).appendTo($confirm_btn_area);
        var $cancel_btn_area = $('<div>').addClass('cancelbtn').appendTo($reminder_btn);
        var $reminder_cancel_panel = $('<div>').addClass('panel').append($TR('pincode_btn_cancel :: cancel')).appendTo($cancel_btn_area);
     
        var listData = [
			{
				name: 'confirm_btn',
				$root: $reminder_confirm_panel,
				callback:null,
			},
			{
				name: 'cancel_btn',
				$root: $reminder_cancel_panel,
				callback:null,
			}
		];
        var focusList = new window.FocusList(listData);
		
		var channelMrl = null;
		var timer = null;
	
        var updateScreen = function() {
            window.Platform.pvr.getUpcommingReminderData(upadateMessage);
            focusList.select(0);
        };
     
        var upadateMessage = function (error, reminderInfo) {
            $reminder_channel_name.text(reminderInfo.serviceName);
            $reminder_event_name.text(reminderInfo.name);
            var date_start = window.Format.formatDateLayoutTS( reminderInfo.time, name ) ;
            var time_start = window.Format.formatTimeLayoutTS( reminderInfo.time, name ) ;
//            var date_stop = window.Format.formatDateLayoutTS((reminderInfo.time + reminderInfo.duration), name ) ;
            var time_stop = window.Format.formatTimeLayoutTS( (reminderInfo.time + reminderInfo.duration), name ) ;
            $reminder_event_dateTime.text( date_start + " " + time_start + " - " + time_stop );
            window.Platform.pvr.clearUpcommingReminder( );
	    channelMrl = reminderInfo.mrl;
            console.log("-> the reminded channelMrl: " + channelMrl);
	    timer = setInterval(switchToChannel, 30*1000);
        };
		
        var upadateMessageWithoutSwitching = function (error, reminderInfo) {
            $reminder_channel_name.text(reminderInfo.serviceName);
            $reminder_event_name.text(reminderInfo.name);
            var date_start = window.Format.formatDateLayoutTS( reminderInfo.time, name ) ;
            var time_start = window.Format.formatTimeLayoutTS( reminderInfo.time, name ) ;
//            var date_stop = window.Format.formatDateLayoutTS((reminderInfo.time + reminderInfo.duration), name ) ;
            var time_stop = window.Format.formatTimeLayoutTS( (reminderInfo.time + reminderInfo.duration), name ) ;
            $reminder_event_dateTime.text( date_start + " " + time_start + " - " + time_stop );
            window.Platform.pvr.clearUpcommingReminder();
        };
        		
	var switchToChannel = function () {
		console.log("-> switching to the channel on which the upcoming reminder was set.");
		if (channelMrl !== window.Platform.dvb.ChannelCurrent.mrl) {
		clearInterval( timer );           
		window.AppManager.back();
		var liveTV = window.AppManager.getWidget("Live TV");
		liveTV.playChannel( channelMrl );
	    }
	    else {
		clearInterval( timer );           
		window.AppManager.back();	    	
	    }
	};
     
        self.onKey = function() {
            return true;
        };
        
        self.onShow = function() {
            updateScreen();
        };
     
        self.onHide = function() {
            $reminder_channel_name.text("");
            $reminder_event_name.text("");
            $reminder_event_dateTime.text("");
        } ;
		
		self.onKeyLEFT = function() {            
            focusList.selectPrev();
        };
		
		self.onKeyRIGHT = function() {
            focusList.selectNext();
        };

        self.onKeyOK = function() {
            var focusListname = focusList.getSelectedName();            
			if (focusListname == 'confirm_btn') {
                console.log("Confirm button in " + name + " is pressed");
				switchToChannel();
            }
			else if (focusListname == 'cancel_btn') {
                console.log("Cancel button in " + name + " is pressed");
		clearInterval( timer );
                window.AppManager.back();
            }
        };
        
        self.onKeyEXIT = function () {
            window.AppManager.back();
        };
        
        self.deleteTheReminder = function () {
		window.Platform.pvr.getUpcommingReminderData(upadateMessageWithoutSwitching);
        };
        
        return self;
    };

    window.AppManager.addScene(name, ReminderPopDlg);
})();