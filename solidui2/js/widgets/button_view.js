"use strict"; (function() {

    var ButtonView = new window.Class(
    
        // parent
        window.Widget,
        
        // constructor
        function(description) {
            Widget.call(this, description.name);
        
            this.$root.addClass('button_view');
            this.$panel = $('<div>').addClass('panel').appendTo(this.$root);
            this.$name = $('<div>').addClass('name').append($TR(description.name)).appendTo(this.$panel);
            this.callback = description.callback;
        },
        
        // prototype
        {
            activate: function(flag) {
                if (flag == this.active) {
                    return;
                }
        
                this.active = flag;
                this.$root.toggleClass('active', flag);
        
                if (flag) {
                    this.callback && this.callback();
                } else {
                    //
                }
            },
    
    	}
    
    );

	window.ButtonView = ButtonView ;

})( ) ;
