document.getElementById('sendBtn').addEventListener('click', async () => {
    const formData = {
        name: document.querySelector('[name="name"]').value,
        country: document.querySelector('[name="country"]').value,
        quote: document.querySelector('[name="quotebody"]').value
    };

    if (formData.quote == "") {
        alert("Quote cannot be empty !");
    }
    else {
        try {
            const response = await fetch('/', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(formData)
            });
    
            if (response.ok) {
                alert('Quote submitted successfully!');
                window.location.href = '/quotes';
            } else {
                alert('Submission failed');
            }
    
        } catch (error) {
            console.error('Error:', error);
            alert('Connection error');
        }
    }
});

const navToggle = document.getElementById('navToggle');
const navMenu = document.getElementById('navMenu');

navToggle.addEventListener('click', () => {
    navMenu.classList.toggle('active');
    navToggle.classList.toggle('active');
});
