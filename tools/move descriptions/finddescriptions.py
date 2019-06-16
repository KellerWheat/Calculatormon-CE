from lxml import html
import requests
page = requests.get('https://pokemondb.net/move/generation/1')
tree = html.fromstring(page.content)


inputlinks = tree.xpath('//a[@class="ent-name"]')

links = []

for link in inputlinks:
    links.append('https://pokemondb.net' + link.attrib['href'])

descriptions = []

for link in links:
    page = requests.get(link)
    tree = html.fromstring(page.content)
    descriptions.append(tree.xpath('//td[@class="cell-med-text"]/text()')[2][:-1])
