"use strict"; 
(function() {

    var name = "PIN Code";

    var PinCodes = function() {
        var parent = new window.MainMenuScreen(name);
        var self = Object.create(parent);
        self.$root.attr('id', 'pincode');

        var oldinputDescription = {
            name: 'pincode_oldpassword :: oldpasswordname',
            size: 6,
            type: 'password',
        };
        var $pincode = $('<div>').appendTo(self.$root);
        var $pincode_first_lable = $('<div>').appendTo($pincode);
        var old_input = new window.InputView(oldinputDescription);
        old_input.$root.appendTo($pincode_first_lable);
        old_input.show();

        var newinputDescription = {
            name: 'pincode_newpassword :: newpasswordname',
            size: 6,
            type: 'password',
        };
        var $pincode_second_lable = $('<div>').appendTo($pincode);
        var new_input = new window.InputView(newinputDescription);
        new_input.$root.appendTo($pincode_second_lable);
        new_input.show();

        var confirminputDescription = {
            name: 'pincode_confirmpassword :: confirmpasswordname',
            size: 6,
            type: 'password',
        };
        var $pincode_third_lable = $('<div>').appendTo($pincode);
        var confirm_input = new window.InputView(confirminputDescription);
        confirm_input.$root.appendTo($pincode_third_lable);
        confirm_input.show();

        var $pincode_forth_lable = $('<div>').addClass('messagelable').appendTo($pincode);
        var $pincode_message = $('<div>').addClass('message').appendTo($pincode_forth_lable);

        var $pincode_fifth_lable = $('<div>').appendTo($pincode);
        var $pincode_left_area = $('<div>').addClass('column_left').appendTo($pincode_fifth_lable);
        var $pincode_right_area = $('<div>').addClass('column_right').appendTo($pincode_fifth_lable);
        var $pincode_confirm_panel = $('<div>').addClass('panel').append($TR('pincode_btn_confirm :: confirm')).appendTo($pincode_left_area);
        var $pincode_cancel_panel = $('<div>').addClass('panel').append($TR('IPPV_btn_cancel :: cancel')).appendTo($pincode_right_area);

        var $pincode_last_lable = $('<div>').addClass('remindlable').appendTo($pincode);
        var $redwarning = $('<div>').addClass('redwarning').appendTo($pincode_last_lable);
        var $redwarningtext = $('<div>').addClass('redwarningtext').append($TR('pincode_redwarning :: redwarning')).appendTo($pincode_last_lable);

        var listData = [
        {
            name: 'oldPasswordInput',
            $root: $pincode_first_lable,
            callback: _.bind(old_input.activate, old_input),
        },
        {
            name: 'newPasswordInput',
            $root: $pincode_second_lable,
            callback: _.bind(new_input.activate, new_input),
        },
        {
            name: 'confirmPasswordInput',
            $root: $pincode_third_lable,
            callback: _.bind(confirm_input.activate, confirm_input),
        },
        {
            name: 'pincode_btn_confirm',
            $root: $pincode_confirm_panel,
            callback: null,
        },
        {
            name: 'pincode_btn_cancel',
            $root: $pincode_cancel_panel,
            callback: null,
        }];
        var focusList = new window.FocusList(listData);
        
        var handleSmcEvent = function(param){
		 				if(param.smcStatus == 0){
		 						self.onKeyBACK();
		 				}
		 		};

        var flag = 0;
        var updateScreen = function() {
            flag = 0;
            focusList.select(0);
            old_input.clear();
            new_input.clear();
            confirm_input.clear();
            $pincode_message.text("");
            window.Platform.ca.setNotifySmcStatusCallback(handleSmcEvent);
        }

        self.onKey = function() {
            return true;
        };

        self.onShow = function() {
            updateScreen();
        };

        self.onKeyUP = function() {
            $pincode_message.text("");
            if (flag == 1) {
                old_input.clear();
                new_input.clear();
                confirm_input.clear();
                flag = 0;
            }
            focusList.selectPrev();
            console.log("focusList.getSelected()" + focusList.getSelected());
        };

        self.onKeyDOWN = function() {
            $pincode_message.text("");
            if (flag == 1) {
                old_input.clear();
                new_input.clear();
                confirm_input.clear();
                flag = 0;
            }
            focusList.selectNext();
        };

        self.onKeyNUMBER = function(num) {
            if (focusList.getSelectedName() == 'oldPasswordInput') {
                old_input.appendChar(num.toString());
            } else if (focusList.getSelectedName() == 'newPasswordInput') {
                new_input.appendChar(num.toString());
            } else if (focusList.getSelectedName() == 'confirmPasswordInput') {
                confirm_input.appendChar(num.toString());
            }
        };
        var changePassword = function(oldpin, newpin, confirmpin) {
            if (newpin == confirmpin) {
                var pincode = {
                    OldPinCode: oldpin,
                    NewPinCode: newpin
                };
                var ret = Platform.ca.ChangePin(pincode);
                return ret.retVal;
            } else {
                return 100;
            }
        };
        var ChangePassword = function() {
            flag = 1;
            var return_value = changePassword(old_input.getText(), new_input.getText(), confirm_input.getText());
            $("#pincode  .message").css('color', return_value ? 'red': 'green');
            switch (return_value) {
            case 0:
                {
                    $pincode_message.text(TR('Pincode :: PIN has been changed'));
                    break;
                }
            case 3:
                {
                    $pincode_message.text(TR('IPPV_Unidentified Card!'));
                    break;
                }
            case 4:
                {
                    $pincode_message.text(TR('Pincode :: Invalid PIN'));
                    break;
                }
            case 100:
                {
                    $pincode_message.text(TR('Pincode :: Confirm PIN unmatch'));
                    break;
                }
            }
        };

        self.onKeyOK = function() {
            if (focusList.getSelectedName() == 'pincode_btn_confirm') {
                ChangePassword();
            } else if (focusList.getSelectedName() == 'pincode_btn_cancel') {
            		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
                window.AppManager.back();
            }

        };
        
        self.onKeyBACK = 
        self.onKeyLEFT = function() {
		window.Platform.ca.setNotifySmcStatusCallback(function(st){});
		window.AppManager.back();
        }

        self.onKeyRED = function() {
            if (focusList.getSelectedName() == 'oldPasswordInput') {
                old_input.removeChar();
            } else if (focusList.getSelectedName() == 'newPasswordInput') {
                new_input.removeChar();
            } else if (focusList.getSelectedName() == 'confirmPasswordInput') {
                confirm_input.removeChar();
            }
        };
        return self;

    }
    window.AppManager.addScene(name, PinCodes);
})();