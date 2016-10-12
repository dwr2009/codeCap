"use strict"; (function() {

    var InputItem = window.Class(

    // parent
    window.Widget,

    // constructor
    function(description) {
        Widget.call(this, "Input Item");
        this.$root.addClass('input_item');
        this.$input = $('<div>').addClass('text').appendTo(this.$root);
        this.length = description.size;
        this.ip_section = 0;
        this.accurationLength = description.accurationLength;
        this.type = description.type;
        this.text = new String();
        if (this.type == "ipv4") {
            this.text = "0.0.0.0";
        }
        this.textPass = new String();
        this.showContext = function() {
            if (this.type == 'password') {
                this.$input.text(this.getFormat(this.textPass));
            } else if (this.type == 'decimal') {
                var temp = this.insertDecimal(this.getFormat(this.text), this.length - this.accurationLength, '.');
                this.$input.text(temp);   
            }
            else if(this.type == 'text'){
            	this.$input.text(this.get_Text(this.text));
            }
            else if (this.type == 'ipv4') {
                this.$input.text(this.get_ip(this.text));
            }
            else {
                this.$input.text(this.getFormat(this.text));
            }   
        }
        this.getFormat = function (str) {
            for (var i = str.length; i < this.length; i++)
            {
                str += '-';
            }
            return str;
        }
        this.insertDecimal = function (str1, n, str2) {
            if (str1.length == 0) {
                return str1;
            }
            else if (str1.length <= n) {
                return str1 + str2;
            } else {
                var s1 = str1.substring(0, n);
                var s2 = str1.substring(n, str1.length);
                return s1 + str2 + s2;
            }
        }
        this.get_Text = function( str ){
        	for (var i = str.length; i < this.length; i++)
            {
                str += '';
            }
            return str;
        }
        this.get_ip = function( str )
        {
            
            var ip_array = str.split(".");
            if (ip_array.length != 4) {
                this.text = "0.0.0.0";
                return "0.0.0.0";
            }
            if ( parseInt(ip_array[this.ip_section]) > 255  || parseInt(ip_array[this.ip_section]) < 0 ) {
                ip_array[this.ip_section] = ip_array[this.ip_section].substr(0, ip_array[this.ip_section].length - 1);
            }
            this.text = ip_array.map(Number).join(".").toString();
            return this.text;
        }
        this.show();
    },

    // prototype
    {
        onShow: function() {
            this.showContext();
        },

        appendChar: function(char) {
            if (char.length != 1 || this.text.length == this.length) return;
            if (this.type == "ipv4") {
                var ip_array = this.text.split(".");
                ip_array[this.ip_section] = ip_array[this.ip_section] + char;
                this.text = ip_array.join(".");
                this.showContext();
                return;
            }
            this.text += char;
            this.textPass += '*';

            this.showContext();
        },
		
        //only for ip
        next_section: function()
        {
            if (this.type == 'ipv4') {
                if(this.ip_section < 3)
                {
                    ++this.ip_section;
                    return true;
                }
                return false;
            }
            else
            return false;
        },
        //only for ip 
        back_section: function()
        {
            if ( this.type == 'ipv4') {
                if (this.ip_section > 0) {
                    --this.ip_section;
                    return true;
                }
                return false;
            }
            else
                return false;
            
            
        },
        
	set_text: function( txt )
	{
		if(txt.length > this.length) return;

		this.text = txt;
		
		this.showContext();
		//this.$cursor.hide();
	},


        removeChar: function() {
             if (this.type == "ipv4") {
                var ip_array = this.text.split(".");
                ip_array[this.ip_section] = ip_array[this.ip_section].substring(0, ip_array[this.ip_section].length - 1);
                this.text = ip_array.join(".");
                this.showContext();
                return;
            }
            this.text = this.text.substring(0, this.text.length - 1);
            this.textPass = this.textPass.substring(0, this.textPass.length - 1);

            this.showContext();
        },

        clear: function() {
            this.text = new String();
            this.textPass = new String();
            this.showContext();
        },
		    
        getText: function ( ) {
            if (this.type == 'decimal') {
                if (this.text.length <= this.length - this.accurationLength)
                    return this.text;
                else
                    return this.insertDecimal(this.text, this.length - this.accurationLength, '.');
            }
            else {
                return this.text ;
            }
        },
        text_length: function(){
        	return this.text.length;
        },
		}

	) ;

	window.InputItem = InputItem ;

})( ) ;
