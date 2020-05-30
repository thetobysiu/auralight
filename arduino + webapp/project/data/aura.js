const postUrl = '/';
let colorProfile = {
    "TypicalLEDStrip": 0xFFB0F0,
    "TypicalPixelString": 0xFFE08C,
    "UncorrectedColor": 0xFFFFFF
};
$(document).ready(function(){
    for (let key in colorProfile) {
        $("select[name=temp]").append('<option value=' + colorProfile[key] + '>' + key + '</option>');
    }

    $('a[data-toggle="tab"]').on('show.bs.tab', function(e) {
        localStorage.setItem('activeTab', $(e.target).attr('href'));
    });
    let activeTab = localStorage.getItem('activeTab');
    if(activeTab){
        $('#myTab a[href="' + activeTab + '"]').tab('show');
    }

    $("#updateBrightness").click(function(e){
        e.preventDefault();
        let brightness = $("input[name=brightness]").val();
        $.ajax({
            type: "POST",
            url: postUrl,
            data: { brightness: brightness },
            success: function(){
                location.reload();
            },
            error: function(){
                alert('failed');
            }
        });
    });
    $("#updateTemp").click(function(e){
        e.preventDefault();
        let temp = $("select[name=temp]").val();
        $.ajax({
            type: "POST",
            url: postUrl,
            data: { temp: temp },
            success: function(){
                location.reload();
            },
            error: function(){
                alert('failed');
            }
        });
    });
    $("#updateMood").click(function(e){
        e.preventDefault();
        let breathColor = parseInt($("input[name=breathColor]").val().substr(1), 16);
        let breathDelay = $("input[name=breathDelay]").val();
        let startBrightness = $("input[name=startBrightness]").val();
        let endBrightness = $("input[name=endBrightness]").val();
        $.ajax({
            type: "POST",
            url: postUrl,
            data: {
                breathColor: breathColor,
                breathDelay: breathDelay,
                startBrightness: startBrightness,
                endBrightness: endBrightness
            },
            success: function(){
                location.reload();
            },
            error: function(){
                alert('failed');
            }
        });
    });
    $("#updateUDP").click(function(e){
        e.preventDefault();
        $.ajax({
            type: "POST",
            url: postUrl,
            data: { mood: '' },
            success: function(){
                location.reload();
            },
            error: function(){
                alert('failed');
            }
        });
    });
});
