// https://observablehq.com/@d3/zoomable-bar-chart@233
function _1(md){return(
md`# Zoomable Bar Chart

This bar chart uses D3’s zoom behavior on the *x*-axis. Double-click on the bar chart below or use the mouse wheel (or pinch) to zoom. This example is contrived—you don’t need zooming if you can easily display all the bars at once.`
)}

function _chart(d3,width,height,zoom,data,x,y,xAxis,yAxis)
{
  const svg = d3.create("svg")
      .attr("viewBox", [0, 0, width, height])
      .call(zoom);

  svg.append("g")
      .attr("class", "bars")
      .attr("fill", "steelblue")
    .selectAll("rect")
    .data(data)
    .join("rect")
      .attr("x", d => x(d.name))
      .attr("y", d => y(d.value))
      .attr("height", d => y(0) - y(d.value))
      .attr("width", x.bandwidth());

  svg.append("g")
      .attr("class", "x-axis")
      .call(xAxis);

  svg.append("g")
      .attr("class", "y-axis")
      .call(yAxis);

  return svg.node();
}


function _zoom(margin,width,height,d3,x,xAxis){return(
function zoom(svg) {
  const extent = [[margin.left, margin.top], [width - margin.right, height - margin.top]];

  svg.call(d3.zoom()
      .scaleExtent([1, 8])
      .translateExtent(extent)
      .extent(extent)
      .on("zoom", zoomed));

  function zoomed(event) {
    x.range([margin.left, width - margin.right].map(d => event.transform.applyX(d)));
    svg.selectAll(".bars rect").attr("x", d => x(d.name)).attr("width", x.bandwidth());
    svg.selectAll(".x-axis").call(xAxis);
  }
}
)}

async function _data(d3,FileAttachment){return(
d3.csvParse(await FileAttachment("alphabet.csv").text(), ({letter, frequency}) => ({name: letter, value: +frequency})).sort((a, b) => b.value - a.value)
)}

function _x(d3,data,margin,width){return(
d3.scaleBand()
    .domain(data.map(d => d.name))
    .range([margin.left, width - margin.right])
    .padding(0.1)
)}

function _y(d3,data,height,margin){return(
d3.scaleLinear()
    .domain([0, d3.max(data, d => d.value)]).nice()
    .range([height - margin.bottom, margin.top])
)}

function _xAxis(height,margin,d3,x){return(
g => g
    .attr("transform", `translate(0,${height - margin.bottom})`)
    .call(d3.axisBottom(x).tickSizeOuter(0))
)}

function _yAxis(margin,d3,y){return(
g => g
    .attr("transform", `translate(${margin.left},0)`)
    .call(d3.axisLeft(y))
    .call(g => g.select(".domain").remove())
)}

function _height(){return(
500
)}

function _margin(){return(
{top: 20, right: 0, bottom: 30, left: 40}
)}

function _d3(require){return(
require("d3@6")
)}

export default function define(runtime, observer) {
  const main = runtime.module();
  function toString() { return this.url; }
  const fileAttachments = new Map([
    ["alphabet.csv", {url: new URL("./files/lineaBaseDatos.csv", import.meta.url), mimeType: null, toString}]
  ]);
  main.builtin("FileAttachment", runtime.fileAttachments(name => fileAttachments.get(name)));
  main.variable(observer()).define(["md"], _1);
  main.variable(observer("chart")).define("chart", ["d3","width","height","zoom","data","x","y","xAxis","yAxis"], _chart);
  main.variable(observer("zoom")).define("zoom", ["margin","width","height","d3","x","xAxis"], _zoom);
  main.variable(observer("data")).define("data", ["d3","FileAttachment"], _data);
  main.variable(observer("x")).define("x", ["d3","data","margin","width"], _x);
  main.variable(observer("y")).define("y", ["d3","data","height","margin"], _y);
  main.variable(observer("xAxis")).define("xAxis", ["height","margin","d3","x"], _xAxis);
  main.variable(observer("yAxis")).define("yAxis", ["margin","d3","y"], _yAxis);
  main.variable(observer("height")).define("height", _height);
  main.variable(observer("margin")).define("margin", _margin);
  main.variable(observer("d3")).define("d3", ["require"], _d3);
  return main;
}
