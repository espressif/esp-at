$(function() {
    sessionStorage.clear();
    eventBind();
})
function eventBind() {
    $(document).on("click", "#login-btn", function() {
        login();
    })
    $(document).on("click", "a.forget-password", function() {
        $("#forget-title").modal("show");
    })
}

function login() {
    var password = $("#password").val();
    if (isEmpty(password)) {
        toastr.warning("Please enter the password!");
        return false;
    }

    $.ajax({
        type: "post",
        url: "/loginpwd",
        headers: {
            "Content-Type": "application/json",
        },
        data: "loginpwd=" + password,
        dataType: "json",
        success: function(resp){
            if(resp.state == SUCCESS_CODE){
                window.location = "/index.html";
                sessionStorage.setItem(LOGIN_USER, password);
            }else{
                toastr.error(ERROR_INFO[resp.error_code]);
                sessionStorage.clear();
            }
        },
        error: function(resp){
            toastr.error(ERROR_INFO[resp.error_code]);
            sessionStorage.clear();
        }
    });

}
function isEmpty(str) {
    if (str === "" || str === null || str === undefined) {
        return true;
    } else {
        return false;
    }
}

