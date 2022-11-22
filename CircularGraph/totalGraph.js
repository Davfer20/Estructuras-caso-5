var arrayData = new Array();
var archivoTxt = new XMLHttpRequest();
var fileRuta = 'index.txt';
archivoTxt.open("GET",fileRuta,false);
archivoTxt.send(null);
var txt = archivoTxt.responseText;
for (var i = 0; i < txt.length;i++){
    arrayData.push(txt[i]);
}
arrayData.forEach(function(data)
{
    console.log(data);
    datasum += parseInt(data);
});
if (dataSum == 0)
{
    console.log('Inserte dato en el index.txt')
}
else{
    console.log('La suma de los datos del txt es: '+ dataSum)
}

 