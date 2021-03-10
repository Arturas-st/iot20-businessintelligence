let tabledata = document.getElementById('tabledata')


fetch(" https://arturasfunctions.azurewebsites.net/api/GetAllFromCosmosDb")
.then(res => res.json())
.then(data => {
    for(let row of data){
        tabledata.innerHTML += `<tr><td>${row.id}</td><td>${row.deviceId}</td><td>${row.ts}</td><td>${row.temp}</td><td>${row.hum}</td>`
    }
})