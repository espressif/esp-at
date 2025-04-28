// Get the link to the previous page
var previousUrl = document.referrer;

// Check if document.referrer is available
if (previousUrl) {
    // Check if the previous page and the current one are in the same domain
    if (new URL(previousUrl).origin === window.location.origin) {
      fetch(previousUrl)
        .then((response) => response.text())
        // Get the title of the previous page
        .then(() => {
          // Get the target of the previous page
          var matches = previousUrl.match(/esp32[0-9a-z]*/);
          if (matches && matches.length) {
            var previousTarget = matches[0];
          }

          // Get the target of the current page
          var currentUrl = window.location.href;
          matches = currentUrl.match(/esp32[0-9a-z]*/);
          if (matches && matches.length) {
            var currentTarget = matches[0];
          }

          // Get the link of the upper level page
          var upperLevelUrl = previousUrl.slice(0, previousUrl.lastIndexOf("/"));

          // Get the target of the upper level page
          var upperLevelTarget = previousTarget;

          // Replace the target of the upper level page with the target of the current page
          if (upperLevelTarget && currentTarget) {
              upperLevelUrl = upperLevelUrl.replace(upperLevelTarget, currentTarget);
          }

          // Replace the current link with the upper level link
          if (upperLevelUrl) {
              setTimeout(() => {
                  window.location.href = upperLevelUrl;
                }, 5000); // Delay redirect by 5000 ms (5 seconds)
          }
    })
        .catch((error) => {
          console.error("Error:", error);
        });
    } else {
      console.error("Referrer URL is from a different domain.");
    }
  } else {
    console.error("No referrer URL available.");
  }
