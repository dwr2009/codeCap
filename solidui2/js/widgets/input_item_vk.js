"use strict"; (function() {

    var InputItemVK = window.Class(

    // parent
    window.Widget,

    // constructor
    function(description) {
        Widget.call(this, "Input Item VK");
        this.$root.addClass('input_item_vk');
        this.$input = $('<input>').addClass('inputText_vk').appendTo(this.$root);
        this.length = description.size;
        this.accurationLength = description.accurationLength;
        this.type = description.type;
        this.text = new String();
        this.showContext = function() {
	        	if (this.type == 'password') {
              this.$input.attr("type","password");
							this.$input.val(this.get_Text(this.text));
            }
        		else if(this.type == 'text' ){
            	this.$input.attr("type","text");
            	this.$input.val(this.get_Text(this.text));
            }  
        }
        
        this.get_Text = function( str ){
        	for (var i = str.length; i < this.length; i++)
            {
                str += '';
            }
            return str;
        }
        this.show();
    },

    // prototype
    {
        onShow: function() {
        	if(this.$input.value != undefined){
        		this.showContext();
        	}
        },

        appendChar: function(char) {
            if (char.length != 1 || this.text.length == this.length) return;
            this.text = this.$input.val();
            this.text += char;
            this.showContext();
        },
				set_text: function( txt )
				{
					if(txt.length > this.length) return;
					this.text = txt;
					this.showContext();
				},

        removeChar: function() {
        		this.text = this.$input.val();
            this.text = this.text.substring(0, this.text.length - 1);
            this.showContext();
        },

        clear: function() {
        		this.$input[0].value = new String();
            this.text = new String();
            this.showContext();
        },
		    
        getText: function ( ) {
        	if(this.$input[0].value.length > this.length){
        		this.$input[0].value = this.$input[0].value.substring(0,  this.length - 1);
        	}
            	return this.$input[0].value ;
        },
        
        text_length: function(){
        		return this.text.length;
        },
        
        setFocus: function(isFocus){
        		if(isFocus){
	        		this.$input[0].focus();
	        	}
	        	else{
	        		this.$input[0].blur();
	        	}
        },
		}

	) ;

	window.InputItemVK = InputItemVK ;

})( ) ;
