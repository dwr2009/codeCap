"use strict"; 
(function() {

    var name = "Parental Rating";

    var ParentalRatings = function() {

        var switchParentalRating = function(item) {
            console.log("Selected :rating", item.name);
            setCurrentRating(item.value);
        };

        var parent = new window.MainMenuScreen(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'parentalrating');

        var labelDescription = {
            name: 'parentalrating_currentratingname :: currentratingname',
            value: ''
        };

        var $parentalrating = $('<div>').appendTo(self.$root);
        var $parentalrating_first_lable = $('<div>').appendTo($parentalrating);
        var parentalrating_currentrating = new window.LabelView(labelDescription);
        parentalrating_currentrating.$root.appendTo($parentalrating_first_lable);
        parentalrating_currentrating.show();

        var selectDescription = {
            name: "parental rating screen :: rating",
            items: [{
                name: 'parental rating screen :: 4',
                value: '4',
            },
            {
                name: 'parental rating screen :: 5',
                value: '5',
            },
            {
                name: 'parental rating screen :: 6',
                value: '6',
            },
            {
                name: 'parental rating screen :: 7',
                value: '7',
            },
            {
                name: 'parental rating screen :: 8',
                value: '8',
            },
            {
                name: 'parental rating screen :: 9',
                value: '9',
            },
            {
                name: 'parental rating screen :: 10',
                value: '10',
            },
            {
                name: 'parental rating screen :: 11',
                value: '11',
            },
            {
                name: 'parental rating screen :: 12',
                value: '12',
            },
            {
                name: 'parental rating screen :: 13',
                value: '13',
            },
            {
                name: 'parental rating screen :: 14',
                value: '14',
            },
            {
                name: 'parental rating screen :: 15',
                value: '15',
            },
            {
                name: 'parental rating screen :: 16',
                value: '16',
            },
            {
                name: 'parental rating screen :: 17',
                value: '17',
            },
            {
                name: 'parental rating screen :: 18',
                value: '18',
            },
            ],
            action: switchParentalRating,
        };
        var $parentalrating_second_lable = $('<div>').addClass('parentalrating_lable').appendTo($parentalrating);
        var newrating = new window.SelectView(selectDescription, selectDescription.action);
        newrating.$root.appendTo($parentalrating_second_lable);
        newrating.show();

        var inputDescription = {
            name: 'parentalrating_passwordname :: passwordname',
            size: 6,
            type: 'password',
        };
        var $parentalrating_third_lable = $('<div>').appendTo($parentalrating);
        var input = new window.InputView(inputDescription);
        input.$root.appendTo($parentalrating_third_lable);
        input.show();

        var $parentalrating_forth_lable = $('<div>').addClass('messagelable').appendTo($parentalrating);
        var $parentalrating_message = $('<div>').addClass('message').appendTo($parentalrating_forth_lable);

        var $parentalrating_fifth_lable = $('<div>').appendTo($parentalrating);
        var $parentalrating_left_area = $('<div>').addClass('column_left').appendTo($parentalrating_fifth_lable);
        var $parentalrating_right_area = $('<div>').addClass('column_right').appendTo($parentalrating_fifth_lable);
        var $parentalrating_confirm_panel = $('<div>').addClass('panel').append($TR('pincode_btn_confirm :: confirm')).appendTo($parentalrating_left_area);
        var $parentalrating_cancel_panel = $('<div>').addClass('panel').append($TR('IPPV_btn_cancel :: cancel')).appendTo($parentalrating_right_area);

        var $parentalrating_last_lable = $('<div>').addClass('remindlable').appendTo($parentalrating);
        var $redwarning = $('<div>').addClass('redwarning').appendTo($parentalrating_last_lable);
        var $redwarningtext = $('<div>').addClass('redwarningtext').append($TR('parentalrating_redwarning :: redwarning')).appendTo($parentalrating_last_lable);
        var listData = [{
            name: 'parentalrating_newrating',
            $root: $parentalrating_second_lable,
            callback: null,
        },
        {
            name: 'parentalrating_inputpassword',
            $root: $parentalrating_third_lable,
            callback: _.bind(input.activate, input),
        },
        {
            name: 'parentalrating_btn_confirm',
            $root: $parentalrating_confirm_panel,
            callback: null,
        },
        {
            name: 'parentalrating_btn_cancel',
            $root: $parentalrating_cancel_panel,
            callback: null,
        }];
        var focusList = new window.FocusList(listData);
        var flag = 0;
        var getRatingData = function() {
            var rating = Platform.ca.GetRating();
            if (rating.retVal == 0) {
                parentalrating_currentrating.setText(rating.rating);
            } else {
                parentalrating_currentrating.setText("");
            }
        };

        var select_view = null;
        var currentrating = 0;
        var setCurrentRating = function(ratingvalue) {
            currentrating = ratingvalue;
        }
        var getCurrentRating = function() {
            return currentrating;
        }

        var setNewRating = function() {
            flag = 1;
            $parentalrating_message.text("setNewRating");
            var rating = {
                rating: getCurrentRating(),
                pinCode: input.getText()
            };
            var ret = Platform.ca.SetRating(rating);
            $("#parentalrating  .message").css('color', ret.retVal ? 'red': 'green');
            switch (ret.retVal) {
            case 0:
                $parentalrating_message.text(TR('parentalrating :: Teleview rating changed successfully'));
                getRatingData();
                break;
            case 3:
                $parentalrating_message.text(TR('IPPV_Unidentified Card!'));
                break;
            case 4:
                $parentalrating_message.text(TR('Pincode :: Invalid PIN'));
                break;
            case 0x0E:
                $parentalrating_message.text(TR('parentalrating :: Invalid teleview rating'));
                break;
            default:
                $parentalrating_message.text(TR('parentalrating :: Teleview rating changed failed'));
                break;
            }
        }
        
				var handleSmcEvent = function(param){
			 			if(param.smcStatus == 0){
			 					self.onKeyBACK();
			 			}
			 	};
			 	
        var updateScreen = function() {
            flag = 0;
            focusList.select(0);
            newrating.changeSelection(0);
            input.clear();
            getRatingData();
            $parentalrating_message.text("");
            window.Platform.ca.setNotifySmcStatusCallback(handleSmcEvent);
        };
        
				self.onKey = function() {
            return true;
        };
        
        self.onShow = function() {
            updateScreen();
        };

        self.onKeyNUMBER = function(num) {
            if (focusList.getSelectedName() == 'parentalrating_inputpassword') {
                input.appendChar(num.toString());
            }
        };

        self.onKeyUP = function() {
            if (select_view) {
                return;
            }
            if (flag == 1) {
                input.clear();
                flag = 0;
            }
            $parentalrating_message.text("");
            focusList.selectPrev();
        };

        self.onKeyDOWN = function() {
            if (select_view) {
                return;
            }
            $parentalrating_message.text("");
            if (flag == 1) {
                input.clear();
                flag = 0;
            }
            focusList.selectNext();
        };

        self.onKeyOK = function() {
            var focusListname = focusList.getSelectedName();
            switch (focusListname) {
            case 'parentalrating_newrating':
                {
                    if (!select_view) {
                        select_view = newrating;
                        select_view.activate(true);
                    } else {
                        select_view.activate(false);
                        select_view = null;
                    }
                    break;
                }
            case 'parentalrating_btn_confirm':
                {
                    setNewRating();
                    break;
                }
            case 'parentalrating_btn_cancel':
                {
                		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
                    window.AppManager.back();
                    break;
                }
            }

        };

				self.onKeyBACK = function() {
            window.Platform.ca.setNotifySmcStatusCallback(function(st){});
            window.AppManager.back();
        };
        
        self.onKeyLEFT = function() {
            if (!select_view) {
            		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
                window.AppManager.back();
                return;
            }
            console.log("select_view" + select_view);
            select_view.selectPrev();
        };

        self.onKeyRIGHT = function() {
            if (!select_view) {
                return;
            }
            console.log("select_view" + select_view);
            select_view.selectNext();
        };

        self.onKeyRED = function() {
            input.removeChar();
        };
        return self;
    };

    window.AppManager.addScene(name, ParentalRatings);
})();